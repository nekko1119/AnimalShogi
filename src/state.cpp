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
    }

    state::state() BOOST_NOEXCEPT
    : board_{initial_placement_tag{}}
    {
    }

	// TODO : テストコード
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
            std::vector<point> arounds = target->search(pt);
            remove_outside_points(arounds);
            remove_same_turn(arounds, target->get_turn(), board_);
            arounds.shrink_to_fit();
            return arounds;
        }

        ASHOGI_LOG_TRIVIAL(error) << "駒のない座標が渡された";
        throw std::runtime_error("the point of coordinate is empty");
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
}