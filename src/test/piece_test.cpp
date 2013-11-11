#include <piece.h>
#include <gtest/gtest.h>
using namespace animal_shogi;

TEST(piece_test, example)
{
    piece const p{turn::BLACK, ptype::CHICK};
    auto const move_points = p.calc_moves({3, 2});
    EXPECT_EQ(point(3, 1), move_points[0]);
    EXPECT_EQ("CH", p.str());
}

TEST(piece_test, calc_moves)
{
    piece p{turn::BLACK, ptype::CHICK};
    std::vector<point> expect = {{0, -1}};
    std::vector<point> actual = p.calc_moves({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::BLACK, ptype::CHICK};
    expect = {{2, 2}};
    actual = p.calc_moves({2, 3});
    EXPECT_EQ(expect, actual);

    p = piece{turn::WHITE, ptype::CHICK};
    expect = {{0, 1}};
    actual = p.calc_moves({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::BLACK, ptype::ELEPHANT};
    expect = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
    actual = p.calc_moves({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::BLACK, ptype::GIRAFFE};
    expect = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};
    actual = p.calc_moves({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::BLACK, ptype::LION};
    expect = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    actual = p.calc_moves({0, 0});
    EXPECT_EQ(expect, actual);

    p = piece{turn::BLACK, ptype::HEN};
    expect = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {0, 1}};
    actual = p.calc_moves({0, 0});
    EXPECT_EQ(expect, actual);
}

TEST(piece_test, str)
{
    piece p = {turn::BLACK, ptype::CHICK};
    EXPECT_EQ("CH", p.str());

    p = {turn::WHITE, ptype::CHICK};
    EXPECT_EQ("ch", p.str());

    p = {turn::BLACK, ptype::ELEPHANT};
    EXPECT_EQ("EL", p.str());

    p = {turn::WHITE, ptype::ELEPHANT};
    EXPECT_EQ("el", p.str());

    p = {turn::BLACK, ptype::GIRAFFE};
    EXPECT_EQ("GI", p.str());

    p = {turn::WHITE, ptype::GIRAFFE};
    EXPECT_EQ("gi", p.str());

    p = {turn::BLACK, ptype::HEN};
    EXPECT_EQ("HE", p.str());

    p = {turn::WHITE, ptype::HEN};
    EXPECT_EQ("he", p.str());

    p = {turn::BLACK, ptype::LION};
    EXPECT_EQ("LI", p.str());

    p = {turn::WHITE, ptype::LION};
    EXPECT_EQ("li", p.str());
}