#include "board.h"
#include <boost/range/algorithm/copy.hpp>
#include <algorithm>

namespace animal_shogi
{
    board::board(initial_placement_tag)
    {
        // どうぶつしょうぎの初期配置
        /*
        象ラ麒
        ＿ひ＿
        ＿ひ＿
        麒ラ象
        */
        board_[1][1] = {turn::WHITE, ptype::GIRAFFE};
        board_[1][2] = {turn::WHITE, ptype::LION};
        board_[1][3] = {turn::WHITE, ptype::ELEPHANT};
        board_[2][2] = {turn::WHITE, ptype::CHICK};
        board_[3][2] = {turn::BLACK, ptype::CHICK};
        board_[4][1] = {turn::BLACK, ptype::ELEPHANT};
        board_[4][2] = {turn::BLACK, ptype::LION};
        board_[4][3] = {turn::BLACK, ptype::GIRAFFE};
    }

    std::array<board::piece_type, board::MAX_ROW * board::MAX_COLUMN> board::serialize() const
    {
        std::array<piece_type, MAX_ROW * MAX_COLUMN> ar;
        auto it = std::begin(ar);
        for (auto const& row : board_)
        {
            it = boost::copy(row, it);
        }
        return ar;
    }

    board::piece_type const& board::get(point p) const
    {
        return board_[p.y()][p.x()];
    }

    board::piece_type& board::get(point p)
    {
        return board_[p.y()][p.x()];
    }

    std::string board::str() const
    {
        std::string str;
        std::for_each(std::begin(board_) + 1, std::end(board_) - 1, [&str](inner_type const& in)
        {
            str.append("|");
            std::for_each(std::begin(in) + 1, std::end(in) - 1, [&str](boost::optional<piece> const& p)
            {
                str.append((p ? p->str() : "   ") + "|");
            });
            str.append("\n");
        });
        return str;
    }

    bool is_within_board(point const& p)
    {
        // どうぶつしょうぎで有効な座標は横が[1, 3]かつ、縦が[1, 4]
        return (0 < p.x() && p.x() < board::MAX_ROW - 1) && (0 < p.y() && p.y() < board::MAX_COLUMN - 1);
    }
}