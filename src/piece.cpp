#include "piece.h"
#include <unordered_map>

namespace animal_shogi
{
    namespace
    {
        using moves_t = std::unordered_map<ptype, std::vector<point>>;

        // 駒の動きの表
        moves_t const piece_movement_table =
        {
            {ptype::CHICK, {{0, -1}}},
            {ptype::ELEPHANT, {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}}},
            {ptype::GIRAFFE, {{0, -1}, {-1, 0}, {1, 0}, {0, 1}}},
            {ptype::LION, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}},
            {ptype::HEN, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {0, 1}}}
        };

        void reverse_points(std::vector<point>& points)
        {
            for (auto& p : points)
            {
                p *= {1, -1};
            }
        }
    }

    piece::piece(turn turn, ptype ptype)
    : turn_(turn), ptype_(ptype)
    {
    }

    std::vector<point> piece::calc_moves(point const& current) const
    {
        auto points = piece_movement_table.at(ptype_);
#if BOOST_WORKAROUND(BOOST_MSVC_FULL_VER, == 180021114)
        if (turn_ == turn::WHITE)
#else
        if (is_white(turn_))
#endif
        {
            reverse_points(points);
        }

        for (auto& p : points)
        {
            p += current;
        }
        return points;
    }

    std::string piece::str() const
    {
        auto str = to_string(ptype_);
        return get_turn_mark(turn_) + str;
    }

    bool piece::operator==(piece const& rhs) const
    {
        return turn_ == rhs.turn_ && ptype_ == rhs.ptype_;
    }

    bool piece::operator!=(piece const& rhs) const
    {
        return !(*this == rhs);
    }
}