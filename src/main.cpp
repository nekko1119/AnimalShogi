#include <array>
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
    std::string const win_eval_file_name{"win_eval_table.csv"};
    std::string const lose_eval_file_name{"lose_eval_table.csv"};
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
void special(int argc, char** argv);

int main(int argc, char** argv)
{
    try
    {
        auto log = animal_shogi::logging{};

        // モードを選択
        char input = '\0';
        while (input != 'p' && input != 'l' && input != 's')
        {
            std::cout << "play(p) learn(l) special(s) >";
            std::cin >> input;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (input == 'p')
        {
            play(argc, argv);
        }
        else if (input == 'l')
        {
            learn(argc, argv);
        }
        else if (input == 's')
        {
            special(argc, argv);
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

    ASHOGI_LOG_TRIVIAL(info) << "先手" << black_str << " 読み深さ" << depth << " 後手" << white_str << " 読み深さ" << depth2;
    ASHOGI_LOG_TRIVIAL(info) << "対戦回数" << last;

    for (int i = 0; i < last; ++i)
    {
        ASHOGI_LOG_TRIVIAL(info) << i + 1 << "戦目";
        auto const res = animal_shogi::game{black_player, white_player}();
        ++results[static_cast<int>(res)];
    }
    animal_shogi::write_csv(std::cout, results);
}

void special(int argc, char** argv)
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

    std::vector<double> const rand_piece_eval_table = {1.0, 8.77283, 7.61874, 8.64285, 3.95307};
    auto black_player = create_player(black_str, animal_shogi::alphabeta{animal_shogi::random_piece_advantage{rand_piece_eval_table}, depth});
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
        std::ifstream ifs{win_eval_file_name};
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

        // スレッドを作成し、半分回数ずつ実行する
        // 先手後手差が出ないように、先手半分、後手半分で実行する
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
            // 先手後手入れ変えている
            auto const res = animal_shogi::game{white_player, black_player}();
            ++results[static_cast<int>(!res)];
        }

        auto other_results = future.get();

        // それぞれのスレッドの勝敗結果を合計する
        decltype(results) sum_results = {{0, 0, 0}};
        calculate(std::begin(results), std::end(results), std::begin(other_results), std::begin(sum_results));

        // 勝敗と乱数テーブルを記録する
        std::ofstream result_ofs{result_file_name, std::ios::out | std::ios::app};
        animal_shogi::write_csv(result_ofs, sum_results);

        std::ofstream win_eval_ofs{win_eval_file_name, std::ios::out | std::ios::app};
        animal_shogi::write_csv(win_eval_ofs, sum_results[0] < sum_results[1] ? white_player_rand_eval_table : black_player_rand_eval_table);

        std::ofstream lose_eval_ofs{lose_eval_file_name, std::ios::out | std::ios::app};
        animal_shogi::write_csv(lose_eval_ofs, sum_results[0] < sum_results[1] ? black_player_rand_eval_table : white_player_rand_eval_table);
    }
}