#include "utility/point.hpp"
#include <gtest/gtest.h>
using namespace animal_shogi;

TEST(point_test, example)
{
    point p{1, 2};
    EXPECT_EQ(1, p.x());
    p.y(3);
    p += {2, 2};
    EXPECT_TRUE(p == point(3, 5));
    testing::StaticAssertTypeEq<point::value_type, int>();
}

TEST(point_test, constuct)
{
    point p{1, 2};
    EXPECT_EQ(1, p.x());
    EXPECT_EQ(2, p.y());
}

TEST(point_test, copy_ctor)
{
    point p1{1, 2};
    point p2{p1};
    EXPECT_EQ(1, p2.x());
    EXPECT_EQ(2, p2.y());
}

TEST(point_test, assignment)
{
    point p1{1, 2};
    point p2{3, 4};
    p2 = p1;
    EXPECT_EQ(1, p2.x());
    EXPECT_EQ(2, p2.y());
}

TEST(point_test, accessor)
{
    point p{1, 2};
    EXPECT_EQ(1, p.x());
    EXPECT_EQ(2, p.y());
    p.x(3);
    EXPECT_EQ(3, p.x());
    p.y(4);
    EXPECT_EQ(4, p.y());
}

TEST(point_test, equality)
{
    point p1{1, 2};
    point p2{3, 4};
    EXPECT_FALSE(p1 == p2);
    EXPECT_TRUE(p1 != p2);
    p2 = p1;
    EXPECT_TRUE(p1 == p2);
    EXPECT_FALSE(p1 != p2);
}

TEST(point_test, add)
{
    point p1{1, 2};
    point p2{3, 4};
    p1 += p2;
    EXPECT_EQ(p1, point(4, 6));
    EXPECT_EQ(p1 + p2, point(7, 10));
}

TEST(point_test, sub)
{
    point p1{1, 2};
    point p2{3, 4};
    p1 -= p2;
    EXPECT_EQ(p1, point(-2, -2));
    EXPECT_EQ(p1 - p2, point(-5, -6));
}

TEST(point_test, mul)
{
    point p1{1, 2};
    point p2{3, 4};
    p1 *= p2;
    EXPECT_EQ(p1, point(3, 8));
    EXPECT_EQ(p1 * p2, point(9, 32));
}