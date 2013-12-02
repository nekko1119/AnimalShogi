#include <piece.h>
#include <gtest/gtest.h>
using namespace animal_shogi;

TEST(piece_test, example)
{
    piece const p{turn::BLACK, ptype::CHICK};
    auto const move_points = p.search({3, 2});
    EXPECT_EQ(point(3, 1), move_points[0]);
    EXPECT_EQ("+CH", p.str());
}

TEST(piece_test, search)
{
    piece p{turn::BLACK, ptype::CHICK};
    std::vector<point> expect = {{0, -1}};
    std::vector<point> actual = p.search({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::BLACK, ptype::CHICK};
    expect = {{2, 2}};
    actual = p.search({2, 3});
    EXPECT_EQ(expect, actual);

    p = piece{turn::WHITE, ptype::CHICK};
    expect = {{0, 1}};
    actual = p.search({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::BLACK, ptype::ELEPHANT};
    expect = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
    actual = p.search({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::BLACK, ptype::GIRAFFE};
    expect = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};
    actual = p.search({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::BLACK, ptype::LION};
    expect = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    actual = p.search({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::BLACK, ptype::HEN};
    expect = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {0, 1}};
    actual = p.search({0, 0});
    EXPECT_EQ(expect, actual);
}

TEST(piece_test, str)
{
    piece p = {turn::BLACK, ptype::CHICK};
    EXPECT_EQ("+CH", p.str());

    p = {turn::WHITE, ptype::CHICK};
    EXPECT_EQ("-CH", p.str());

    p = {turn::BLACK, ptype::ELEPHANT};
    EXPECT_EQ("+EL", p.str());

    p = {turn::WHITE, ptype::ELEPHANT};
    EXPECT_EQ("-EL", p.str());

    p = {turn::BLACK, ptype::GIRAFFE};
    EXPECT_EQ("+GI", p.str());

    p = {turn::WHITE, ptype::GIRAFFE};
    EXPECT_EQ("-GI", p.str());

    p = {turn::BLACK, ptype::HEN};
    EXPECT_EQ("+HE", p.str());

    p = {turn::WHITE, ptype::HEN};
    EXPECT_EQ("-HE", p.str());

    p = {turn::BLACK, ptype::LION};
    EXPECT_EQ("+LI", p.str());

    p = {turn::WHITE, ptype::LION};
    EXPECT_EQ("-LI", p.str());
}

TEST(piece_test, equality)
{
    piece p1{turn::BLACK, ptype::CHICK};
    piece p2{turn::BLACK, ptype::CHICK};
    piece p3{turn::WHITE, ptype::CHICK};
    piece p4{turn::BLACK, ptype::ELEPHANT};
    piece p5{turn::WHITE, ptype::ELEPHANT};
    EXPECT_TRUE(p1 == p1);
    EXPECT_TRUE(p1 == p2);
    EXPECT_TRUE(p1 != p3);
    EXPECT_TRUE(p1 != p4);
    EXPECT_TRUE(p1 != p5);
}