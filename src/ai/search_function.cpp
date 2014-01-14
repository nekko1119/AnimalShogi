#include "search_function.h"

#include <algorithm>
#include <limits>
#include <random>
#include <unordered_map>
#include <vector>
#include <boost/range/algorithm/find.hpp>
#include "../utility/logging.h"

namespace animal_shogi
{
    namespace
    {
        auto engine = std::mt19937{std::random_device{}()};
    }

    minimax::minimax(eval_func_type eval_func, std::size_t depth)
    : eval_func_{std::move(eval_func)},
    depth_{depth}
    {
        if (depth < 1)
        {
            ASHOGI_LOG_TRIVIAL(error) << "再帰深度は１以上でなければならない";
            throw std::invalid_argument{"depth should be at least 1"};
        }
    }

    int minimax::operator()(state st)
    {
        state_ = st;
        auto const moves = enumerate_movable_pieces(st, st.current_turn());
        auto const move = *(execute(st, depth_).second);
        auto const it = boost::find(moves, move);
        return std::distance(std::begin(moves), it);
    }

    minimax::result_type minimax::execute(state const& st, std::size_t depth) const
    {
        auto const moves = enumerate_movable_pieces(st, st.current_turn());
        if (depth == 0 || moves.empty())
        {
            auto const sign = state_.current_turn() == st.current_turn() ? 1 : -1;
            return {sign * eval_func_(st), boost::none};
        }

        using move_list_t = std::unordered_map<int, std::vector<boost::optional<movement>>>;
        using value_type = move_list_t::value_type;
        move_list_t best;
        for (auto const& it : moves)
        {
            state next;
            if (it.from)
            {
                next = st.update_from_board_copy(*(it.from), it.to);
            }
            else
            {
                next = st.update_from_cap_pc_copy(it.to, it.pc);
            }

            ASHOGI_LOG_TRIVIAL(debug) << "depth : " << depth_ - depth + 1 << ", move : " << it.str();
            auto score = execute(next, depth - 1);
            best[score.first].push_back(it);
        }

        // 評価値が一番高い指し手を検索
        auto first = std::max_element(std::begin(best), std::end(best), [](value_type const& l, value_type const& r)
        {
            return l.first < r.first;
        });

        // その中から１つランダムで選ぶ
        std::uniform_int_distribution<> dist{0, static_cast<int>(first->second.size() - 1)};
        auto const index = dist(engine);

        return {first->first, *(first->second[index])};
    }
}