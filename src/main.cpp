#include <array>
#include <functional>
#include <iostream>
#include <boost/utility/string_ref.hpp>
#include "ai/evaluation_function.h"
#include "ai/search_function.h"
#include "game.h"
#include "player/ai.h"
#include "player/human.h"
#include "utility/exchange.hpp"
#include "utility/logging.h"
#include "utility/command_line_parser.h"

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

template <typename Stream>
void write_result(Stream& out, std::array<int, 3> const& results)
{
    bool is_first = true;
    out << "black,white,draw\n";
    for (auto const& it : results)
    {
        // 最初だけカンマを出力しない
        if (!animal_shogi::exchange(is_first, false))
        {
            out << ",";
        }
        out << it;
    }
}

int main(int argc, char** argv)
{
    try
    {
        auto log = animal_shogi::logging{};

        animal_shogi::command_line_parser parser{argc, argv};

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
        write_result(std::cout, results);
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