﻿#include <array>
#include <functional>
#include <fstream>
#include <iostream>
#include <future>
#include <boost/range/algorithm/transform.hpp>
#include <boost/utility/string_ref.hpp>
#include "ai/evaluation_function.h"
#include "ai/search_function.h"
#include "game.h"
#include "player/ai.h"
#include "player/human.h"
#include "utility/io_csv.hpp"
#include "utility/logging.h"
#include "utility/command_line_parser.h"

namespace
{
    std::string const eval_file_name{"eval_table.csv"};
    std::string const result_file_name{"result_table.csv"};
}

template <typename... Args>
std::function<void (animal_shogi::state&)> create_player(boost::string_ref const& player, Args&&... args)
{
    if (player == "human")
    {
            return animal_shogi::human{};
    }
    else if (player == "ai")
    {
        return animal_shogi::ai{std::forward<Args>(args)...};
    }
    else
    {
        throw std::invalid_argument{"player name is must be \"human\" or \"ai\""};
    }
}

// TODO : 標準で同じことする関数があるはずなので見つけたら置換する
template <typename ForwardIterator1, typename ForwardIterator2, typename OutputIterator, typename BinaryOperation = std::plus<>>
void calculate(ForwardIterator1 first1, ForwardIterator1 const last1, ForwardIterator2 first2, OutputIterator output, BinaryOperation op = std::plus<>{})
{
    for (; first1 != last1; ++first1, ++first2)
    {
        *output++ = op(*first1, *first2);
    }
}

void play(int argc, char** argv);
void learn(int argc, char** argv);

int main(int argc, char** argv)
{
    try
    {
        auto log = animal_shogi::logging{};


        // 学習かどうか選択
        char input = '\0';
        while (input != 'y' && input != 'n')
        {
            std::cout << "learning mode ? (y/n) >";
            std::cin >> input;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (input == 'n')
        {
            play(argc, argv);
        }
        else if (input == 'y')
        {
            learn(argc, argv);
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "unknown exception" << std::endl;
    }
}

void play(int argc, char** argv)
{
    animal_shogi::command_line_parser parser{argc, argv, false};

    auto const black_str = parser.get<std::string>("black");
    auto const white_str = parser.get<std::string>("white");

    std::size_t depth = 0U;
    std::size_t depth2 = 0U;
    if (black_str == "ai" && white_str == "ai")
    {
        depth = parser.get<std::size_t>("depth");
        depth2 = parser.get<std::size_t>("depth2");
    }
    else
    {
        depth = depth2 = parser.get<std::size_t>("depth");
    }

    auto black_player = create_player(black_str, animal_shogi::alphabeta{animal_shogi::piece_advantage{}, depth});
    auto white_player = create_player(white_str, animal_shogi::alphabeta{animal_shogi::piece_advantage{}, depth2});

    std::array<int, 3> results = {{0, 0, 0}};
    auto const last = parser.get<int>("loop");
    for (int i = 0; i < last; ++i)
    {
        auto const res = animal_shogi::game{black_player, white_player}();
        ++results[static_cast<int>(res)];
    }
    animal_shogi::write_csv(std::cout, results);
}

void learn(int argc, char** argv)
{
    animal_shogi::command_line_parser parser{argc, argv, true};

    auto const depth = parser.get<std::size_t>("depth");
    auto const depth2 = parser.get<std::size_t>("depth2");

    auto const generation = parser.get<int>("generation");

    for (int i = 0; i < generation; ++i)
    {
        std::cout << i + 1 << "generations\n";

        // もし乱数データのファイルが存在するのならばそれを使用し、無ければ乱数を生成して使用する
        std::ifstream ifs{eval_file_name};
        auto const get_random_eval_values = [&ifs]()
        {
            if (ifs)
            {
                auto const csv_data = animal_shogi::read_csv(ifs);
                auto const eval_table = csv_data.back();
                using result_type = animal_shogi::random_piece_advantage::result_type;
                std::vector<result_type> transformed;
                boost::transform(eval_table, std::back_inserter(transformed), [](std::string const& s){ return std::stod(s); });
                return animal_shogi::random_piece_advantage{transformed};
            }
        
            return animal_shogi::random_piece_advantage{};
        };
        //先手後手の乱数テーブルを保存する
        auto const black_player_rand_random_pieces = get_random_eval_values();
        auto const white_player_rand_random_pieces = animal_shogi::random_piece_advantage{};
        auto const black_player_rand_eval_table = black_player_rand_random_pieces.piece_values();
        auto const white_player_rand_eval_table = white_player_rand_random_pieces.piece_values();
    
        auto black_player = animal_shogi::ai{animal_shogi::alphabeta{black_player_rand_random_pieces, depth}};
        auto white_player = animal_shogi::ai{animal_shogi::alphabeta{white_player_rand_random_pieces, depth2}};
    
        ifs.close();

        //スレッドを作成し、半分回数ずつ実行する
        auto const last = parser.get<int>("loop") / 2;
        auto future = std::async([&]()
        {
            std::array<int, 3> results = {{0, 0, 0}};
            for (int i = 0; i < last; ++i)
            {
                auto const res = animal_shogi::game{black_player, white_player}();
                ++results[static_cast<int>(res)];
            }
            return results;
        });

        std::array<int, 3> results = {{0, 0, 0}};
        for (int i = 0; i < last; ++i)
        {
            auto const res = animal_shogi::game{black_player, white_player}();
            ++results[static_cast<int>(res)];
        }

        auto other_results = future.get();

        // それぞれのスレッドの勝敗結果を合計する
        decltype(results) sum_results = {{0, 0, 0}};
        calculate(std::begin(results), std::end(results), std::begin(other_results), std::begin(sum_results));

        // 勝敗と勝った方の乱数テーブルを記録する
        std::ofstream result_ofs{result_file_name, std::ios::out | std::ios::app};
        animal_shogi::write_csv(result_ofs, sum_results);
        std::ofstream eval_ofs{eval_file_name, std::ios::out | std::ios::app};
        animal_shogi::write_csv(eval_ofs, sum_results[0] < sum_results[1] ? white_player_rand_eval_table : black_player_rand_eval_table);
    }
}