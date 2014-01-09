#include "game.hpp"
#include "player/human.h"
#include "utility/logging.h"

int main()
{
    auto log = animal_shogi::logging{};
    animal_shogi::game<animal_shogi::human, animal_shogi::human>{}();
}