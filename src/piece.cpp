#include "piece.h"
#include <algorithm>
#include <functional>
#include <unordered_map>
using namespace animal_shogi;
using namespace std;

namespace
{
    using moves_t = unordered_map<ptype, vector<point>>;

    // 駒の動きのテーブル
    const moves_t piece_moves =
    {
        {ptype::CHICK, {{0, -1}}},
        {ptype::ELEPHANT, {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}}},
        {ptype::GIRAFFE, {{0, -1}, {-1, 0}, {1, 0}, {0, 1}}},
        {ptype::LION, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}}},
        {ptype::HEN, {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {0, 1}}}
    };
}

piece::piece(player player, ptype ptype)
: player_(player), ptype_(ptype)
{
}

vector<point> piece::calc_moves(point const& current) const
{
    auto points = piece_moves.at(ptype_);
    transform(begin(points), end(points), begin(points), [&current](const point& p)
    {
        return p + current;
    });

    return points;
}