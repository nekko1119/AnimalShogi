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
        std::unordered_map<ptype, double> const piece_eval_table =
        {
            {ptype::chick, 1.0},
            {ptype::elephant, 4.0},
            {ptype::giraffe, 4.0},
            {ptype::hen, 6.0},
            {ptype::lion, 8.0}
        };

        std::array<ptype, 4> BOOST_CONSTEXPR_OR_CONST ptype_table = {ptype::chick, ptype::elephant, ptype::giraffe, ptype::lion};
        std::array<ptype, 5> BOOST_CONSTEXPR_OR_CONST all_ptype_table = {ptype::chick, ptype::elephant, ptype::giraffe, ptype::hen, ptype::lion};

        auto engine(std::mt19937{std::random_device{}()});
        auto dist = std::uniform_real_distribution<>{0.0, random_piece_advantage::max_eval_value};

        double const win_eval = 100.0;
        double const lose_eval = -200.0;
    }

    piece_advantage::result_type piece_advantage::operator()(state const& s) const
    {
        result_type state_eval = 0;

        // 盤面の評価
        auto const brd = s.get_board().serialize();
        for (auto const& it : brd)
        {
            if (it)
            {
                state_eval += to_double(*it, s.current_turn());
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
            state_eval += piece_eval_table.at(it) * enemy_cap.get(it);
        }
        
        if (s.has_won(s.current_turn()))
        {
            state_eval += static_cast<result_type>(win_eval);
        }

        if (s.has_won(!s.current_turn()) || s.is_a_draw())
        {
            state_eval += static_cast<result_type>(lose_eval);
        }

        ASHOGI_LOG_TRIVIAL(debug) << "eval : " << state_eval;
        return state_eval;
    }

    double piece_advantage::to_double(piece const& p, turn current) const
    {
        auto const sign = current == p.get_turn() ? 1.0 : -1.0;
        return piece_eval_table.at(p.get_ptype()) * sign;
    }

    random_piece_advantage::random_piece_advantage()
    : random_piece_advantage{{1.0, dist(engine), dist(engine), dist(engine), dist(engine)}}
    {
    }

    random_piece_advantage::random_piece_advantage(std::vector<result_type> const& values)
    {
        if (values.size() != all_ptype_table.size())
        {
            ASHOGI_LOG_TRIVIAL(error) << "駒の種類の数と異なる数が渡された";
            std::range_error{"values's size must be equal to all_ptype_table. (5)"};
        }

        auto first = values.begin();
        for (auto const& it : all_ptype_table)
        {
            piece_values_.insert({it, *first});
            ++first;
        }
    }

    random_piece_advantage::result_type random_piece_advantage::operator()(state const& s) const
    {
        result_type state_eval = 0;

        // 盤面の評価
        auto const brd = s.get_board().serialize();
        for (auto const& it : brd)
        {
            if (it)
            {
                state_eval += to_double(*it, s.current_turn());
            }
        }

        // 持ち駒の評価
        auto const own_cap = s.get_captured_piece(s.current_turn());
        for (auto const& it : ptype_table)
        {
            state_eval += piece_values_.at(it) * own_cap.get(it);
        }
        auto const enemy_cap = s.get_captured_piece(!s.current_turn());
        for (auto const& it : ptype_table)
        {
            state_eval -= piece_values_.at(it) * enemy_cap.get(it);
        }

        if (s.has_won(s.current_turn()))
        {
            state_eval += static_cast<result_type>(win_eval);
        }

        if (s.has_won(!s.current_turn()) || s.is_a_draw())
        {
            state_eval += static_cast<result_type>(lose_eval);
        }

        ASHOGI_LOG_TRIVIAL(debug) << "eval : " << state_eval;
        return state_eval;
    }

    std::vector<random_piece_advantage::result_type> random_piece_advantage::piece_values() const
    {
        std::vector<result_type> eval_table;
        for (auto const& it : all_ptype_table)
        {
            eval_table.push_back(piece_values_.at(it));
        }
        return eval_table;
    }

    double random_piece_advantage::to_double(piece const& p, turn current) const
    {
        auto const sign = current == p.get_turn() ? 1.0 : -1.0;
        return piece_values_.at(p.get_ptype()) * sign;
    }
}