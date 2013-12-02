#include "board.h"
#include <gtest/gtest.h>
using namespace animal_shogi;

TEST(board_test, example)
{
}

TEST(board_test, get)
{
    board b{initial_placement_tag()};
    EXPECT_EQ(piece(turn::BLACK, ptype::LION), *b.get({2, 4}));
    EXPECT_TRUE(boost::none == b.get({1, 2}));
    b.get({2, 4}) = boost::none;
    b.get({1, 2}) = piece{turn::BLACK, ptype::LION};
    EXPECT_EQ(piece(turn::BLACK, ptype::LION), *b.get({1, 2}));
    EXPECT_TRUE(boost::none == b.get({2, 4}));
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