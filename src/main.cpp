#include <iostream>
#include "ai/evaluation_function.h"
#include "ai/search_function.h"
#include "game.h"
#include "player/ai.h"
#include "player/human.h"
#include "utility/logging.h"
#include "utility/command_line_parser.h"

int main(int argc, char** argv)
{
    try
    {
        auto log = animal_shogi::logging{};
        animal_shogi::command_line_parser parser{argc, argv};

        if (parser.get<std::string>("black") == "human" && parser.get<std::string>("white") == "human")
        {
            animal_shogi::game{animal_shogi::human{}, animal_shogi::human{}}();
        }
        else if (parser.get<std::string>("black") == "human" && parser.get<std::string>("white") == "ai")
        {
            std::size_t depth = parser.get<std::size_t>("depth");
            animal_shogi::game{animal_shogi::human{}, animal_shogi::ai{animal_shogi::alphabeta{animal_shogi::piece_advantage{}, depth}}}();
        }
        else if (parser.get<std::string>("black") == "ai" && parser.get<std::string>("white") == "human")
        {
            std::size_t depth = parser.get<std::size_t>("depth");
            animal_shogi::game{animal_shogi::ai{animal_shogi::alphabeta{animal_shogi::piece_advantage{}, depth}}, animal_shogi::human{}}();
        }
        else if (parser.get<std::string>("black") == "ai" && parser.get<std::string>("white") == "ai")
        {
            std::size_t depth = parser.get<std::size_t>("depth");
            std::size_t depth2 = parser.get<std::size_t>("depth2");
            animal_shogi::game{animal_shogi::ai{
                animal_shogi::alphabeta{animal_shogi::piece_advantage{}, depth}},
                animal_shogi::ai{animal_shogi::alphabeta{animal_shogi::piece_advantage{}, depth2}}}();
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