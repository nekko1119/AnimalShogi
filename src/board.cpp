#include "board.h"
#include <algorithm>
using namespace animal_shogi;
using namespace std;

board::board()
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

string board::str() const
{
    string str;
    for_each(begin(board_) + 1, end(board_) - 1, [&str](inner_type const& in)
    {
        str.append("|");
        for_each(begin(in) + 1, end(in) - 1, [&str](boost::optional<piece> const& p)
        {
            str.append((p ? p->str() : "   ") + "|");
        });
        str.append("\n");
    });
    return str;
}