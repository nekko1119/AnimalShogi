#include "captured_piece.h"
#include <stdexcept>
#include <gtest/gtest.h>
using namespace animal_shogi;

TEST(captured_piece_test, example)
{
    captured_piece cp;
    EXPECT_EQ(cp.get(ptype::CHICK), 0);
    cp.add(ptype::CHICK);
    cp.add(ptype::CHICK);
    EXPECT_EQ(cp.get(ptype::CHICK), 2);
    cp.remove(ptype::CHICK);
    EXPECT_EQ(cp.get(ptype::CHICK), 1);
}

TEST(captured_piece_test, construct)
{
    captured_piece cp;
    EXPECT_EQ(cp.get(ptype::CHICK), 0);
    EXPECT_EQ(cp.get(ptype::ELEPHANT), 0);
    EXPECT_EQ(cp.get(ptype::GIRAFFE), 0);
    EXPECT_EQ(cp.get(ptype::LION), 0);
    EXPECT_THROW(cp.get(ptype::HEN), std::invalid_argument);
}

TEST(captured_piece_test, add)
{
    captured_piece cp;
    cp.add(ptype::CHICK);
    cp.add(ptype::CHICK);
    EXPECT_THROW(cp.add(ptype::CHICK), std::runtime_error);
    cp.add(ptype::ELEPHANT);
    cp.add(ptype::ELEPHANT);
    cp.add(ptype::GIRAFFE);
    EXPECT_THROW(cp.add(ptype::HEN), std::invalid_argument);
    EXPECT_EQ(cp.get(ptype::CHICK), 2);
    EXPECT_EQ(cp.get(ptype::ELEPHANT), 2);
    EXPECT_EQ(cp.get(ptype::GIRAFFE), 1);
    EXPECT_EQ(cp.get(ptype::LION), 0);
}