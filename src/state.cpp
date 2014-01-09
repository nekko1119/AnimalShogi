#include "state.h"
#include <stdexcept>
#include <type_traits>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include "utility/logging.h"

namespace animal_shogi
{
    namespace
    {
        // 盤の範囲外の座標を取り除く
        void remove_outside_points(std::vector<point>& points)
        {
            boost::remove_erase_if(points, [](point const& p){ return !is_within_board(p); });
        }

        // 味方の駒がある座標を取り除く
        void remove_same_turn(std::vector<point>& arounds, turn trn, board const& b)
        {
            boost::remove_erase_if(arounds, [&](point const& p){ return b.at(p) && b[p]->get_turn() == trn; });
        }

        std::vector<point> remove_invalid_points(point const& pt, board::piece_type const& target, board const& brd)
        {
            std::vector<point> arounds = target->search(pt);
            remove_outside_points(arounds);
            remove_same_turn(arounds, target->get_turn(), brd);
            arounds.shrink_to_fit();
            return arounds;
        }

        void emplace_back_if_piece_exists(state const& s, turn const& trn, point const& p, std::vector<point>& empty_points, std::vector<movement>& movable_pieces)
        {
            // 全てのマスにつき、駒があり、かつターンが同じか調べる
            auto const pc = s.get_board().at({p.x(), p.y()});
            if (!pc)
            {
                empty_points.emplace_back(p.x(), p.y());
                return;
            }

            if (pc->get_turn() != trn)
            {
                return;
            }

            // 駒があるなら移動できるマスを全て列挙する
            auto const move_list = s.search({p.x(), p.y()});
            for (auto const& it : move_list)
            {
                movable_pieces.emplace_back(point{p.x(), p.y()}, point{it.x(), it.y()}, *pc);
            }
        }
    }

    state::state() BOOST_NOEXCEPT
    : board_{initial_placement_tag{}}
    {
    }

    void state::update_from_board(point from, point to)
    {
        // 移動可能なマスが存在するか
        auto const points = search(from);
        if (points.empty())
        {
            ASHOGI_LOG_TRIVIAL(error) << "移動できる座標のない駒がfromに渡された";
            throw std::invalid_argument("\"from\" can not move.");
        }

        // その移動可能なマスにtoが含まれているか
        if (boost::find(points, to) == std::end(points))
        {
            ASHOGI_LOG_TRIVIAL(error) << "toには移動できない";
            throw std::invalid_argument("\"from\" can not move to \"to\".");
        }

        // toの座標に駒があれば持ち駒に加える            
        if (auto const dest = board_[to])
        {
            auto const trn = static_cast<turn_type>(board_[from]->get_turn());
            captured_pieces_[trn].add(dest->get_ptype());
        }

        board_[to] = board_[from];
        board_[from] = boost::none;
    }

    void state::update_from_cap_pc(point to, piece pc)
    {
        if (board_[to])
        {
            ASHOGI_LOG_TRIVIAL(error) << "既に駒のある座標に持ち駒を置こうとした";
            throw std::runtime_error("there is a piece already");
        }

        auto const trn = static_cast<turn_type>(pc.get_turn());
        captured_pieces_[trn].remove(pc.get_ptype());
        board_[to] = pc;
    }

    std::vector<point> state::search(point const& pt) const
    {
        if (auto const target = board_.at(pt))
        {
            return remove_invalid_points(pt, target, board_);
        }

        ASHOGI_LOG_TRIVIAL(warning) << "駒のない座標が渡された";
        throw std::runtime_error("the point of coordinate is empty");
    }

    bool state::has_won(turn t) const
    {
        return captured_pieces_[static_cast<turn_type>(t)].get(ptype::lion) != 0;
    }

    board const& state::get_board() const BOOST_NOEXCEPT_OR_NOTHROW
    {
        return board_;
    }

    captured_piece const& state::get_captured_piece(turn trn) const BOOST_NOEXCEPT_OR_NOTHROW
    {
        using turn_t = std::underlying_type_t<turn>;
        return captured_pieces_[static_cast<turn_t>(trn)];
    }

    std::string state::str() const
    {
        std::string str = "second player's captured pieces : ";
        str += captured_pieces_[0].str();
        str += board_.str();
        str += "first player's captured pieces : ";
        str += captured_pieces_[1].str();
        return str;
    }

    std::vector<movement> enumerate_movable_pieces(state const& s, turn trn)
    {
        // 次の番に指せる手のリスト
        std::vector<movement> movable_pieces;
        // 空のマスのリスト
        std::vector<point> empty_points;

        // 盤にある自駒の全ての移動候補(指し手)を追加する
        for (int i = 1; i < board::max_row - 1; ++i)
        {
            for (int j = 1; j < board::max_column - 1; ++j)
            {
                emplace_back_if_piece_exists(s, trn, {i, j}, empty_points, movable_pieces);
            }
        }

        // 空いているマスに持ち駒を打つ手を追加する
        std::array<ptype, 4> const ptype_table = {ptype::chick, ptype::elephant, ptype::giraffe, ptype::giraffe};
        auto const& cap_pc = s.get_captured_piece(trn);
        for (auto const& pc : ptype_table)
        {
            if (!cap_pc.is_empty(pc))
            {
                for (auto const& pt : empty_points)
                {
                    movable_pieces.push_back({boost::none, {pt.x(), pt.y()}, {trn, pc}});
                }
            }
        }

        return movable_pieces;
    }
}