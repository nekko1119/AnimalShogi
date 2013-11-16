#include "piece.h"
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <functional>
#include <locale>
#include <unordered_map>
#include <utility>
using namespace animal_shogi;
using namespace std;

namespace
{
    using moves_t = unordered_map<ptype, pair<vector<point>, string>>;

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
    auto points = piece_movement_table.at(ptype_).first;
    if (is_white(turn_))
    {
        reverse_points(points);
    }
    boost::for_each(points, [&current](point& p){ p += current; });
    return points;
}

string piece::str() const
{
    auto str = piece_movement_table.at(ptype_).second;
    return is_black(turn_) ? str : to_lower(move(str));
}