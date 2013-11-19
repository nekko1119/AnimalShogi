#ifndef ANIMAL_SHOGI_UTILITY_TO_STRING_HPP
#define ANIMAL_SHOGI_UTILITY_TO_STRING_HPP

#include <string>
#include <type_traits>
#include <utility>

namespace animal_shogi
{
    template <typename T>
    inline std::string to_string(std::remove_reference_t<T>&& value) = delete;
}

#endif