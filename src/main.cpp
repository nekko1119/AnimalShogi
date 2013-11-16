#include "board.h"
#include <iostream>

int main()
{
    std::cout << animal_shogi::board::get().str() << std::endl;
}