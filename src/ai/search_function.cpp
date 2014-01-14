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
            return {eval_func_(st), boost::none};
        }

        std::pair<int, boost::optional<movement>> max{std::numeric_limits<int>::min(), boost::none};
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
            if (max.first < -score.first)
            {
                max.first = -score.first;
                max.second = it;
            }
        }

        return max;
    }

    alphabeta::alphabeta(eval_func_type eval_func, std::size_t depth)
    : eval_func_{std::move(eval_func)},
    depth_{depth}
    {
        if (depth < 1)
        {
            ASHOGI_LOG_TRIVIAL(error) << "再帰深度は１以上でなければならない";
            throw std::invalid_argument{"depth should be at least 1"};
        }
    }

    int alphabeta::operator()(state st)
    {
        state_ = st;
        auto const moves = enumerate_movable_pieces(st, st.current_turn());
        auto const move = *(execute(st, depth_, std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max()).second);
        auto const it = boost::find(moves, move);
        return std::distance(std::begin(moves), it);
    }

    alphabeta::result_type alphabeta::execute(state const& st, std::size_t depth, int alpha, int beta) const
    {
        auto const moves = enumerate_movable_pieces(st, st.current_turn());
        if (depth == 0 || moves.empty())
        {
            return {eval_func_(st), boost::none};
        }

        std::pair<int, boost::optional<movement>> max{std::numeric_limits<int>::min(), boost::none};
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
            auto const score = execute(next, depth - 1, -beta, -alpha);
            if (beta <= alpha)
            {
                return {alpha, it};
            }

            if (max.first < -score.first)
            {
                max.first = -score.first;
                max.second = it;
                alpha = std::max(alpha, -score.first);
            }
        }
        return max;
    }
}