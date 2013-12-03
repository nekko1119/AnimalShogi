#include <memory>
#include "utility/logging.h"

#include <iostream>// debug
#include "state.h"// debug

int main()
{
#if BOOST_WORKAROUND(BOOST_GCC, < 40900)
    std::unique_ptr<animal_shogi::logging> log{new animal_shogi::logging{}};
#else
    auto log = std::make_unique<animal_shogi::logging>();
#endif
    animal_shogi::state s;
    std::cout << s.str();
}