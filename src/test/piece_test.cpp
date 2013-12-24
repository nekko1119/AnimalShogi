#include <piece.h>
#include <gtest/gtest.h>
using namespace animal_shogi;

TEST(piece_test, example)
{
    piece const p{turn::black, ptype::chick};
    auto const move_points = p.search({3, 2});
    EXPECT_EQ(point(3, 1), move_points[0]);
    EXPECT_EQ("+CH", p.str());
    EXPECT_EQ(turn::black, p.get_turn());
}

TEST(piece_test, search)
{
    piece p{turn::black, ptype::chick};
    std::vector<point> expect = {{0, -1}};
    std::vector<point> actual = p.search({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::black, ptype::chick};
    expect = {{2, 2}};
    actual = p.search({2, 3});
    EXPECT_EQ(expect, actual);

    p = piece{turn::white, ptype::chick};
    expect = {{0, 1}};
    actual = p.search({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::black, ptype::elephant};
    expect = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
    actual = p.search({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::black, ptype::giraffe};
    expect = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};
    actual = p.search({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::black, ptype::lion};
    expect = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    actual = p.search({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::black, ptype::hen};
    expect = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {0, 1}};
    actual = p.search({0, 0});
    EXPECT_EQ(expect, actual);
}

TEST(piece_test, str)
{
    piece p = {turn::black, ptype::chick};
    EXPECT_EQ("+CH", p.str());

    p = {turn::white, ptype::chick};
    EXPECT_EQ("-CH", p.str());

    p = {turn::black, ptype::elephant};
    EXPECT_EQ("+EL", p.str());

    p = {turn::white, ptype::elephant};
    EXPECT_EQ("-EL", p.str());

    p = {turn::black, ptype::giraffe};
    EXPECT_EQ("+GI", p.str());

    p = {turn::white, ptype::giraffe};
    EXPECT_EQ("-GI", p.str());

    p = {turn::black, ptype::hen};
    EXPECT_EQ("+HE", p.str());

    p = {turn::white, ptype::hen};
    EXPECT_EQ("-HE", p.str());

    p = {turn::black, ptype::lion};
    EXPECT_EQ("+LI", p.str());

    p = {turn::white, ptype::lion};
    EXPECT_EQ("-LI", p.str());
}

TEST(piece_test, get_turn)
{
    piece p1{turn::white, ptype::giraffe};
    EXPECT_EQ(turn::white, p1.get_turn());

    piece p2{turn::black, ptype::elephant};
    EXPECT_EQ(turn::black, p2.get_turn());
}

TEST(piece_test, get_point)
{
    piece p1{turn::white, ptype::giraffe};
    EXPECT_EQ(ptype::giraffe, p1.get_ptype());

    piece p2{turn::black, ptype::elephant};
    EXPECT_EQ(ptype::elephant, p2.get_ptype());
}

TEST(piece_test, equality)
{
    piece p1{turn::black, ptype::chick};
    piece p2{turn::black, ptype::chick};
    piece p3{turn::white, ptype::chick};
    piece p4{turn::black, ptype::elephant};
    piece p5{turn::white, ptype::elephant};
    EXPECT_TRUE(p1 == p1);
    EXPECT_TRUE(p1 == p2);
    EXPECT_TRUE(p1 != p3);
    EXPECT_TRUE(p1 != p4);
    EXPECT_TRUE(p1 != p5);
}