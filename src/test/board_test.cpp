#include "board.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <boost/range/algorithm/equal.hpp>
using namespace animal_shogi;

namespace
{
    std::array<board::piece_type, board::max_column * board::max_row> get_initial_placement_piece_array()
    {
        std::array<board::piece_type, board::max_column * board::max_row> board;
        const auto r = board::max_row;
        board[1 * r + 1] = {turn::white, ptype::giraffe};
        board[1 * r + 2] = {turn::white, ptype::lion};
        board[1 * r + 3] = {turn::white, ptype::elephant};
        board[2 * r + 2] = {turn::white, ptype::chick};
        board[3 * r + 2] = {turn::black, ptype::chick};
        board[4 * r + 1] = {turn::black, ptype::elephant};
        board[4 * r + 2] = {turn::black, ptype::lion};
        board[4 * r + 3] = {turn::black, ptype::giraffe};
        return board;
    }
}

TEST(board_test, example)
{
}

TEST(board_test, default_construct)
{
    board b;
    auto s = b.serialize();
    bool const result = std::all_of(std::begin(s), std::end(s), [](board::piece_type const& op){ return op == boost::none; });
    EXPECT_TRUE(result);
}

TEST(board_test, tag_construct)
{
    board init{initial_placement_tag{}};
    auto const ar = init.serialize();
    auto const are_equal = boost::equal(ar, get_initial_placement_piece_array());
    EXPECT_TRUE(are_equal);
}

TEST(board_test, serialize)
{
    board b1;
    auto s1 = b1.serialize();

    board b2{initial_placement_tag{}};
    auto s2 = b2.serialize();

    EXPECT_EQ(s1.size(), s2.size());
    EXPECT_FALSE(std::equal(std::begin(s1), std::end(s1), std::begin(s2)));

    EXPECT_TRUE(boost::none == s1[2 * board::max_row + 2]);
    EXPECT_EQ(piece(turn::white, ptype::chick), *(s2[2 * board::max_row + 2]));
}

TEST(board_test, at)
{
    board b{initial_placement_tag()};

    // 読み込み
    EXPECT_EQ(piece(turn::black, ptype::lion), *b.at({2, 4}));
    EXPECT_TRUE(boost::none == b.at({1, 2}));

    // 書き込み
    b.at({2, 4}) = boost::none;
    b.at({1, 2}) = piece{turn::black, ptype::lion};

    EXPECT_EQ(piece(turn::black, ptype::lion), *b.at({1, 2}));
    EXPECT_TRUE(boost::none == b.at({2, 4}));
}

TEST(board_test, is_within_board)
{
    EXPECT_TRUE(is_within_board({1, 1}));
    EXPECT_TRUE(is_within_board({3, 1}));
    EXPECT_TRUE(is_within_board({1, 4}));
    EXPECT_TRUE(is_within_board({3, 4}));

    EXPECT_FALSE(is_within_board({0, 0}));
    EXPECT_FALSE(is_within_board({4, 0}));
    EXPECT_FALSE(is_within_board({0, 5}));
    EXPECT_FALSE(is_within_board({4, 5}));
}