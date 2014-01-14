#include <iostream>
#include "ai/evaluation_function.h"
#include "ai/search_function.h"
#include "game.h"
#include "player/ai.h"
#include "player/human.h"
#include "utility/logging.h"

int main()
{
    try
    {
        auto log = animal_shogi::logging{};
        animal_shogi::game{animal_shogi::human{}, animal_shogi::ai{animal_shogi::minimax{animal_shogi::piece_advantage{}, 5}}}();
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