#include <state.h>
#include <stdexcept>
#include <gtest/gtest.h>
using namespace animal_shogi;

TEST(state_test, example)
{
}

TEST(state_test, update_from_board)
{
    state s;
    s.update_from_board({2, 2}, {2, 3});
    
    // 移動元のマスは空になっているか
    auto const& b = s.get_board();
    bool empty = b[{2, 2}] == boost::none;
    EXPECT_TRUE(empty);

    // 移動先のマスに移動しているか
    bool exists = b[{2, 3}] == piece{turn::white, ptype::chick};
    EXPECT_TRUE(exists);
    
    // 取った駒が持ち駒に入っているか
    auto const& captured = s.get_captured_piece(turn::white);
    EXPECT_EQ(1, captured.get(ptype::chick));
}

TEST(state_test, update_from_captured_piece)
{
    state s;
    s.update_from_board({2, 2}, {2, 3});

    s.update_from_cap_pc({2, 2}, {turn::white, ptype::chick});

    // コマが置かれているか
    auto const& b = s.get_board();
    bool exists = b[{2, 2}] == piece{turn::white, ptype::chick};
    EXPECT_TRUE(exists);

    // 持ち駒が減っているか
    auto const& captured = s.get_captured_piece(turn::white);
    EXPECT_EQ(0, captured.get(ptype::chick));
}

TEST(state_test, update_invalid)
{
    state s;

    // 動けるマスが無い
    EXPECT_THROW(s.update_from_board({1, 1}, {2, 2}), std::invalid_argument);

    // toの座標には移動できない
    EXPECT_THROW(s.update_from_board({2, 2}, {1, 2}), std::invalid_argument);
    
    s.update_from_board({2, 2}, {2, 3});

    // 持ち駒を既に駒のある座標に置こうとする
    EXPECT_THROW(s.update_from_cap_pc({2, 3}, {turn::black, ptype::chick}), std::runtime_error);
}

TEST(state_test, search)
{
    state s;

    // (0, 0)に駒はないのでエラー
    EXPECT_THROW(s.search({0, 0}), std::runtime_error);

    // 開いているマスに移動できる
    std::vector<point> expect = {{1, 2}};
    EXPECT_TRUE(expect == s.search({1, 1}));

    // 相手の駒があるマスにも移動できる
    expect = {{2, 2}};
    EXPECT_TRUE(expect == s.search({2, 3}));

    // 移動できるマスがないときは空の配列を返す
    expect = {};
    EXPECT_TRUE(expect == s.search({1, 4}));
}

TEST(state_test, has_won)
{
    state s;
    s.update_from_board({2, 2}, {2, 3});
    s.update_from_board({2, 3}, {2, 4});

    EXPECT_TRUE(s.has_won(turn::white));
    EXPECT_FALSE(s.has_won(turn::black));
}

std::vector<movement> get_white_turn_movable_pieces_in_initial_board()
{
    return
    {
        {point{2, 1}, {1, 2}, {turn::white, ptype::lion}},
        {point{2, 1}, {3, 2}, {turn::white, ptype::lion}},
        {point{1, 1}, {1, 2}, {turn::white, ptype::giraffe}},
        {point{2, 2}, {2, 3}, {turn::white, ptype::chick}}
    };
}

std::vector<movement> get_white_turn_movable_and_dropable_pieces()
{
    return
    {
        {point{2, 1}, {1, 2}, {turn::white, ptype::lion}},
        {point{2, 1}, {2, 2}, {turn::white, ptype::lion}},
        {point{2, 1}, {3, 2}, {turn::white, ptype::lion}},
        {point{3, 1}, {2, 2}, {turn::white, ptype::elephant}},
        {point{1, 1}, {1, 2}, {turn::white, ptype::giraffe}},
        {point{2, 3}, {2, 4}, {turn::white, ptype::chick}},
        {boost::none, {1, 2}, {turn::white, ptype::chick}},
        {boost::none, {3, 2}, {turn::white, ptype::chick}},
        {boost::none, {1, 3}, {turn::white, ptype::chick}},
        {boost::none, {3, 3}, {turn::white, ptype::chick}},
        {boost::none, {2, 2}, {turn::white, ptype::chick}},
    };
}

TEST(state_test, test_enumerate_movable_pieces)
{
    state s;
    {
        auto result = enumerate_movable_pieces(s, turn::white);
        EXPECT_EQ(4, result.size());

        auto const expected = get_white_turn_movable_pieces_in_initial_board();
        const auto actual = std::all_of(std::begin(result), std::end(result), [&expected](movement const& m)
        {
            return std::find(std::begin(expected), std::end(expected), m) != std::end(expected);
        });
        EXPECT_TRUE(actual);
    }
    s.update_from_board({2, 2}, {2, 3});
    {
        auto result = enumerate_movable_pieces(s, turn::white);
        EXPECT_EQ(11, result.size());

        auto const expected = get_white_turn_movable_and_dropable_pieces();
        const auto actual = std::all_of(std::begin(result), std::end(result), [&expected](movement const& m)
        {
            return std::find(std::begin(expected), std::end(expected), m) != std::end(expected);
        });
        EXPECT_TRUE(actual);
    }
}