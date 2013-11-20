#ifndef ANIMAL_SHOGI_UTILITY_TO_STRING_HPP
#define ANIMAL_SHOGI_UTILITY_TO_STRING_HPP

#include <string>

namespace animal_shogi
{
    template <typename T>
    inline std::string to_string(T const& value) = delete;
}

#endif