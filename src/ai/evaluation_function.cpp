#include "evaluation_function.h"

#include <array>
#include <limits>
#include <random>
#include <unordered_map>
#include "../state.h"
#include "../utility/logging.h"

namespace animal_shogi
{
    const double random_piece_advantage::max_eval_value = 10.0;

    namespace
    {
        std::unordered_map<ptype, int> const piece_eval_table =
        {
            {ptype::chick, 1},
            {ptype::elephant, 4},
            {ptype::giraffe, 4},
            {ptype::hen, 5},
            {ptype::lion, 8}
        };

        std::array<ptype, 4> BOOST_CONSTEXPR_OR_CONST ptype_table = {ptype::chick, ptype::elephant, ptype::giraffe, ptype::lion};

        auto engine(std::mt19937{std::random_device{}()});
        auto dist = std::uniform_real_distribution<>{0.0, random_piece_advantage::max_eval_value};
    }

    int piece_advantage::operator()(state const& s) const
    {
        int state_eval = 0;

        // 盤面の評価
        auto const brd = s.get_board().serialize();
        for (auto const& it : brd)
        {
            if (it)
            {
                state_eval += to_int(*it, s.current_turn());
            }
        }

        // 持ち駒の評価
        auto const own_cap = s.get_captured_piece(s.current_turn());
        for (auto const& it : ptype_table)
        {
            state_eval += piece_eval_table.at(it) * own_cap.get(it);
        }
        auto const enemy_cap = s.get_captured_piece(!s.current_turn());
        for (auto const& it : ptype_table)
        {
            state_eval -= piece_eval_table.at(it) * enemy_cap.get(it);
        }
        
        if (s.has_won(s.current_turn()))
        {
            state_eval += 50;
        }

        if (s.has_won(!s.current_turn()) || s.is_a_draw())
        {
            state_eval -= 100;
        }

        ASHOGI_LOG_TRIVIAL(debug) << "eval : " << state_eval;
        return state_eval;
    }

    int piece_advantage::to_int(piece const& p, turn current) const
    {
        auto const sign = current == p.get_turn() ? 1 : -1;
        return piece_eval_table.at(p.get_ptype()) * sign;
    }

    random_piece_advantage::random_piece_advantage()
    : random_piece_advantage{{1.0, dist(engine), dist(engine), dist(engine), dist(engine)}}
    {
    }

    random_piece_advantage::random_piece_advantage(value_type values)
    : piece_values_{std::move(values)}
    {
    }
}