#include <array>
#include <iostream>
#include "ai/evaluation_function.h"
#include "ai/search_function.h"
#include "game.h"
#include "player/ai.h"
#include "player/human.h"
#include "utility/exchange.hpp"
#include "utility/logging.h"
#include "utility/command_line_parser.h"

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
        std::array<int, 3> results = {{0, 0, 0}};
        for (int i = 0; i < parser.get<int>("loop"); ++i)
        {
            animal_shogi::result res;
            std::function<animal_shogi::result ()> gm;
            if (parser.get<std::string>("black") == "human" && parser.get<std::string>("white") == "human")
            {
                gm = animal_shogi::game{animal_shogi::human{}, animal_shogi::human{}};
            }
            else if (parser.get<std::string>("black") == "human" && parser.get<std::string>("white") == "ai")
            {
                std::size_t depth = parser.get<std::size_t>("depth");
                gm = animal_shogi::game{animal_shogi::human{}, animal_shogi::ai{animal_shogi::alphabeta{animal_shogi::piece_advantage{}, depth}}};
            }
            else if (parser.get<std::string>("black") == "ai" && parser.get<std::string>("white") == "human")
            {
                std::size_t depth = parser.get<std::size_t>("depth");
                gm = animal_shogi::game{animal_shogi::ai{animal_shogi::alphabeta{animal_shogi::piece_advantage{}, depth}}, animal_shogi::human{}};
            }
            else if (parser.get<std::string>("black") == "ai" && parser.get<std::string>("white") == "ai")
            {
                std::size_t depth = parser.get<std::size_t>("depth");
                std::size_t depth2 = parser.get<std::size_t>("depth2");
                gm = animal_shogi::game{
                    animal_shogi::ai{animal_shogi::alphabeta{animal_shogi::piece_advantage{}, depth}},
                    animal_shogi::ai{animal_shogi::alphabeta{animal_shogi::piece_advantage{}, depth2}}
                };
            }
            res = gm();
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