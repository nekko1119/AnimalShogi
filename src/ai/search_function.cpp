#include "search_function.h"

#include <boost/range/algorithm/find.hpp>
#include <limits>

namespace animal_shogi
{
    minimax::minimax(eval_func_type eval_func, std::size_t depth)
    : eval_func_{std::move(eval_func)},
    depth_{depth}
    {
        if (depth < 1)
        {
            throw std::invalid_argument{"depth should be at least 1"};
        }
    }

    int minimax::operator()(state const& st) const
    {
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

        result_type best{std::numeric_limits<int>::min(), boost::none};
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

            auto const score = execute(next, depth - 1);

            if (best.first < score.first)
            {
                best.first = score.first;
                best.second = it;
            }
        }
        return best;
    }
}