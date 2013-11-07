#include <piece.h>
#include <gtest/gtest.h>
using namespace animal_shogi;

TEST(piece_test, example)
{
    piece const p{player::BLACK, ptype::CHICK};
    auto const moves = p.calc_moves({3, 2});
    EXPECT_EQ(point(3, 1), moves[0]);
}