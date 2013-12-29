#include "captured_piece.h"
#include <stdexcept>
#include <gtest/gtest.h>
using namespace animal_shogi;

TEST(captured_piece_test, example)
{
    captured_piece cp;
    EXPECT_EQ(cp.get(ptype::chick), 0);
    cp.add(ptype::chick);
    cp.add(ptype::chick);
    EXPECT_EQ(cp.get(ptype::chick), 2);
    cp.remove(ptype::chick);
    EXPECT_EQ(cp.get(ptype::chick), 1);
}

TEST(captured_piece_test, construct)
{
    captured_piece cp;
    EXPECT_EQ(cp.get(ptype::chick), 0);
    EXPECT_EQ(cp.get(ptype::elephant), 0);
    EXPECT_EQ(cp.get(ptype::giraffe), 0);
    EXPECT_EQ(cp.get(ptype::lion), 0);
    EXPECT_THROW(cp.get(ptype::hen), std::invalid_argument);
}

TEST(captured_piece_test, add)
{
    captured_piece cp;
    cp.add(ptype::chick);
    cp.add(ptype::chick);
    EXPECT_THROW(cp.add(ptype::chick), std::runtime_error);
    cp.add(ptype::elephant);
    cp.add(ptype::elephant);
    cp.add(ptype::giraffe);
    EXPECT_THROW(cp.add(ptype::hen), std::invalid_argument);
    EXPECT_EQ(cp.get(ptype::chick), 2);
    EXPECT_EQ(cp.get(ptype::elephant), 2);
    EXPECT_EQ(cp.get(ptype::giraffe), 1);
    EXPECT_EQ(cp.get(ptype::lion), 0);
}

TEST(captured_piece_test, is_empty)
{
    captured_piece cp;
    cp.add(ptype::chick);

    EXPECT_TRUE(cp.is_empty(ptype::elephant));
    EXPECT_FALSE(cp.is_empty(ptype::chick));
}