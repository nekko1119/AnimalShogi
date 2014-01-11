#include "state.h"
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <boost/range/adaptor/filtered.hpp>
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

        std::array<ptype, 4> BOOST_CONSTEXPR_OR_CONST ptype_table = {ptype::chick, ptype::elephant, ptype::giraffe, ptype::lion};

        std::uint16_t encode_captured_pieces(std::array<captured_piece, 2> const& cap)
        {
            /*
            持ち駒の状態を14bitの符号無し整数型にする

            0        2        4        6    7        9        11       13   14       16 (bits)
            |--------|--------|--------|----|--------|--------|--------|----|~~~~~~~~|
            |chick   elephant graffe   lion |chick   elephant graffe   lion blank
            |black turn                     |white turn
            */

            std::uint16_t result = 0U;
            int bit_offset = 0;
            for (auto const& c_it : cap)
            {
                for (auto const& p_it : ptype_table)
                {
                    ASHOGI_LOG_TRIVIAL(debug) << c_it.get(p_it);
                    result |= static_cast<unsigned int>(c_it.get(p_it)) << bit_offset;
                    // 1つの持ち駒は0～2個なので3通り -> 2bit
                    bit_offset += 2;
                }
                // ライオンは0～1個の2通りの1bitで良いので余計な1bitを引く
                --bit_offset;
            }
            return result;
        }

        std::uint64_t encode_piece(board::piece_type const& pc)
        {
            if (!pc)
            {
                return 0ULL;
            }

            if (is_black(pc->get_turn()))
            {
                return static_cast<std::uint64_t>(pc->get_ptype()) + 1ULL; // 駒の無いマスが0なので、区別するために+1している
            }
            else
            {
                return static_cast<std::uint64_t>(pc->get_ptype()) + 1ULL + 5ULL; // 駒は5種類
            }
        }

        std::uint64_t encode_board(board const& brd)
        {
            /*
            盤面の状態を48bitの符号無し整数型にする

            0    4    8    12   16   20   24   28   32   36   40   44   48       64 (bits)
            |----|----|----|----|----|----|----|----|----|----|----|----|~~~~~~~~|
            (1,1)(1,2)(1,3)(1,4)(2,1)(2,2)(2,3)(2,4)(3,1)(3,2)(3,3)(3,4) blank

            */
            std::uint64_t result = 0U;
            int bit_offset = 0;
            for (int i = 1; i < board::max_row - 1; ++i)
            {
                for (int j = 1; j < board::max_column - 1; ++j)
                {
                    ASHOGI_LOG_TRIVIAL(trace) << "(" << i << ", " << j << ") : " << encode_piece(brd[{i, j}]);
                    result |= encode_piece(brd[{i, j}]) << bit_offset;
                    bit_offset += 4;
                }
            }
            return result;
        }

        enum class encode_bit
        {
            turn = 0,
            captured_pieces = 1,
            board = 15,
        };

        std::underlying_type_t<encode_bit> encode_bit_cast(encode_bit enc_bit)
        {
            return static_cast<std::underlying_type_t<encode_bit>>((enc_bit));
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

        // もしひよこが相手の陣地に入る時は成る
        if (board_[from]->get_ptype() == ptype::chick && (to.y() == 1 || to.y() == 4))
        {
            board_[from] = {board_[from]->get_turn(), ptype::hen};
        }

        // toの座標に駒があれば持ち駒に加える            
        if (auto const dest = board_[to])
        {
            auto const trn = static_cast<turn_type>(board_[from]->get_turn());
            auto const ptyp = dest->get_ptype();
            captured_pieces_[trn].add(ptyp == ptype::hen ? ptype::chick : ptyp);
        }

        board_[to] = board_[from];
        board_[from] = boost::none;
        reverse_turn();
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
        reverse_turn();
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

    std::uint64_t state::encode() const
    {
        std::uint64_t result = 0U;
        result |= static_cast<std::uint64_t>(is_white(current_turn_)) << encode_bit_cast(encode_bit::turn);
        result |= encode_captured_pieces(captured_pieces_) << encode_bit_cast(encode_bit::captured_pieces);
        result |= encode_board(board_) << encode_bit_cast(encode_bit::board);
        return result;
    }

    bool state::has_won(turn t) const
    {
        return captured_pieces_[static_cast<turn_type>(t)].get(ptype::lion) != 0;
    }

    turn state::current_turn() const BOOST_NOEXCEPT_OR_NOTHROW
    {
        return current_turn_;
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
        std::string str = board_.str();
        str += "Black's captured pieces : ";
        str += captured_pieces_[0].str();
        str += "White's captured pieces : ";
        str += captured_pieces_[1].str();
        return str;
    }

    void state::reverse_turn()
    {
        current_turn_ = is_black(current_turn_) ? turn::white : turn::black;
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
        auto const& cap_pc = s.get_captured_piece(trn);
        using boost::adaptors::filtered;
        for (auto const& pc : ptype_table | filtered([&cap_pc](ptype p){ return !cap_pc.is_empty(p); }))
        {
            for (auto const& pt : empty_points)
            {
                movable_pieces.push_back({boost::none, {pt.x(), pt.y()}, {trn, pc}});
            }
        }

        return movable_pieces;
    }
}