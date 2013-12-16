#include "board.h"
#include <gtest/gtest.h>
#include <algorithm>
using namespace animal_shogi;

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

    EXPECT_EQ(piece(turn::WHITE, ptype::GIRAFFE), *(ar[1 * board::MAX_ROW + 1]));
    EXPECT_EQ(piece(turn::WHITE, ptype::LION), *(ar[1 * board::MAX_ROW + 2]));
    EXPECT_EQ(piece(turn::WHITE, ptype::ELEPHANT), *(ar[1 * board::MAX_ROW + 3]));
    EXPECT_EQ(piece(turn::WHITE, ptype::CHICK), *(ar[2 * board::MAX_ROW + 2]));
    EXPECT_EQ(piece(turn::BLACK, ptype::ELEPHANT), *(ar[4 * board::MAX_ROW + 1]));
    EXPECT_EQ(piece(turn::BLACK, ptype::LION), *(ar[4 * board::MAX_ROW + 2]));
    EXPECT_EQ(piece(turn::BLACK, ptype::GIRAFFE), *(ar[4 * board::MAX_ROW + 3]));
    EXPECT_EQ(piece(turn::BLACK, ptype::CHICK), *(ar[3 * board::MAX_ROW + 2]));
}

TEST(board_test, serialize)
{
    board b1;
    auto s1 = b1.serialize();

    board b2{initial_placement_tag{}};
    auto s2 = b2.serialize();

    EXPECT_EQ(s1.size(), s2.size());
    EXPECT_FALSE(std::equal(std::begin(s1), std::end(s1), std::begin(s2)));

    EXPECT_TRUE(boost::none == s1[2 * board::MAX_ROW + 2]);
    EXPECT_EQ(piece(turn::WHITE, ptype::CHICK), *(s2[2 * board::MAX_ROW + 2]));
}

TEST(board_test, get)
{
    board b{initial_placement_tag()};

    // 読み込み
    EXPECT_EQ(piece(turn::BLACK, ptype::LION), *b.get({2, 4}));
    EXPECT_TRUE(boost::none == b.get({1, 2}));

    // 書き込み
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