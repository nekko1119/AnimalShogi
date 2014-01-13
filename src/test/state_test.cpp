#include "state.h"
#include <bitset>
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
    auto const& captured_white = s.get_captured_piece(turn::white);
    EXPECT_EQ(1, captured_white.get(ptype::chick));

    // ひよこが相手の陣地に入ったらニワトリに成る
    s.update_from_board({2, 3}, {2, 4});
    exists = b[{2, 4}] == piece{turn::white, ptype::hen};
    EXPECT_TRUE(exists);

    // ニワトリが取られたら、ひよことして持ち駒に入る
    s.update_from_board({3, 4}, {2, 4});
    auto const& captured_black = s.get_captured_piece(turn::black);
    EXPECT_EQ(1, captured_black.get(ptype::chick));
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

void one_cycle(state& s)
{
    s.update_from_board({3, 4}, {3, 3});
    s.update_from_board({1, 1}, {1, 2});
    s.update_from_board({3, 3}, {3, 4});
    s.update_from_board({1, 2}, {1, 1});
}

TEST(state_test, is_a_draw)
{
    state s;
    one_cycle(s);
    EXPECT_FALSE(s.is_a_draw());
    one_cycle(s);
    EXPECT_TRUE(s.is_a_draw());
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

void display_encoded_number(std::uint64_t bits)
{
    // すぐに綺麗な処理を思いつかなかったので力づくなコード
    // 余裕が出来たら綺麗なループに直せないか挑戦する

    std::bitset<64> enc{bits};
    std::cout << "turn : " << enc[0] << std::endl;
    std::cout << "black chick : " << enc[2] << enc[1] << std::endl;
    std::cout << "black elephant : " << enc[4] << enc[3] << std::endl;
    std::cout << "black giraffe : " << enc[6] << enc[5] << std::endl;
    std::cout << "black lion : " << enc[7] << std::endl;
    std::cout << "white chick : " << enc[9] << enc[8] << std::endl;
    std::cout << "white elephant : " << enc[11] << enc[10] << std::endl;
    std::cout << "white giraffe : " << enc[13] << enc[12] << std::endl;
    std::cout << "white lion : " << enc[14] << std::endl;

    std::cout << "|" << enc[18] << enc[17] << enc[16] << enc[15] << "|" << enc[34] << enc[33] << enc[32] << enc[31] << "|" << enc[50] << enc[49] << enc[48] << enc[47] << "|" << std::endl;
    std::cout << "|" << enc[22] << enc[21] << enc[20] << enc[19] << "|" << enc[38] << enc[37] << enc[36] << enc[35] << "|" << enc[54] << enc[53] << enc[52] << enc[51] << "|" << std::endl;
    std::cout << "|" << enc[26] << enc[25] << enc[24] << enc[23] << "|" << enc[42] << enc[41] << enc[40] << enc[39] << "|" << enc[58] << enc[57] << enc[56] << enc[55] << "|" << std::endl;
    std::cout << "|" << enc[30] << enc[29] << enc[28] << enc[27] << "|" << enc[46] << enc[45] << enc[44] << enc[43] << "|" << enc[62] << enc[61] << enc[60] << enc[59] << "|" << std::endl;
}

TEST(state_test, encode)
{
    state s;
    auto const enc1 = s.encode();
    s.update_from_board({2, 3}, {2, 2});
    auto const enc2 = s.encode();
    EXPECT_NE(enc1, enc2);
    display_encoded_number(enc1);
    display_encoded_number(enc2);
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