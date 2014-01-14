#include <iostream>
#include "game.h"
#include "player/human.h"
#include "utility/logging.h"

int main()
{
    try
    {
        auto log = animal_shogi::logging{};
        animal_shogi::game{animal_shogi::human{}, animal_shogi::human{}}();
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