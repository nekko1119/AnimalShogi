#include "piece.h"
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <functional>
#include <locale>
#include <unordered_map>
using namespace animal_shogi;
using namespace std;

namespace
{
    struct piece_info
    {
        piece_info(vector<point> v, string name)
            : moves(v), name(name)
        {
        }

        vector<point> moves;
        string name;
    };

    using moves_t = unordered_map<ptype, piece_info>;

    // 駒の動きの表
    moves_t const piece_movement_table =
    {
        {ptype::CHICK, {{{0, -1}}, "CH"}},
        {ptype::ELEPHANT, {{{-1, -1}, {1, -1}, {-1, 1}, {1, 1}}, "EL"}},
        {ptype::GIRAFFE, {{{0, -1}, {-1, 0}, {1, 0}, {0, 1}}, "GI"}},
        {ptype::LION, {{{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}, "LI"}},
        {ptype::HEN, {{{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {0, 1}}, "HE"}}
    };

    void reverse_points(vector<point>& points)
    {
        boost::for_each(points, [](point& p){ p *= {1, -1}; });
    }

    string to_lower(string range)
    {
        using placeholders::_1;
        using type = string::value_type;
        locale loc;
        boost::transform(range, begin(range), bind(std::tolower<type>, _1, loc));
        return range;
    }
}

piece::piece(turn turn, ptype ptype)
: turn_(turn), ptype_(ptype)
{
}

vector<point> piece::calc_moves(point const& current) const
{
    auto points = piece_movement_table.at(ptype_).moves;
    if (turn_ == turn::WHITE)
    {
        reverse_points(points);
    }
    boost::for_each(points, [&current](point& p){ p += current; });
    return points;
}

string piece::str() const
{
    auto str = piece_movement_table.at(ptype_).name;
    return turn_ == turn::BLACK ? str : to_lower(move(str));
}