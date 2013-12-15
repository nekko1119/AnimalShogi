﻿#include "board.h"
#include <boost/log/trivial.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <algorithm>

namespace animal_shogi
{
    board::board(initial_placement_tag) BOOST_NOEXCEPT_OR_NOTHROW
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
        BOOST_LOG_TRIVIAL(debug) << "board::get const : 引数座標 " << to_string(p);
        return board_[p.y()][p.x()];
    }

    board::piece_type& board::get(point p)
    {
        BOOST_LOG_TRIVIAL(debug) << "board::get : 引数座標 " << to_string(p);
        return board_[p.y()][p.x()];
    }

    board::piece_type const& board::operator[](point p) const
    {
        return get(p);
    }

    board::piece_type& board::operator[](point p)
    {
        return get(p);
    }

    std::string board::str() const
    {
        using boost::adaptors::sliced;
        std::string str;
        for (auto const& row : board_ | sliced(1, MAX_COLUMN - 1))
        {
            str.append("|");
            for (auto const& p : row | sliced(1, MAX_ROW - 1))
            {
                str.append((p ? p->str() : "   ") + "|");
            }
            str.append("\n");
        }
        return str;
    }

    bool is_within_board(point const& p)
    {
        // どうぶつしょうぎで有効な座標は横が[1, 3]かつ、縦が[1, 4]
        bool is_within_row = 0 < p.x() && p.x() < board::MAX_ROW - 1;
        bool is_within_column = 0 < p.y() && p.y() < board::MAX_COLUMN - 1;

        return is_within_row && is_within_column;
    }
}