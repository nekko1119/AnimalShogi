// C++14で入るstd::exchange関数
#ifndef ANIMAL_SHOGI_UTILITY_EXCHANGE_HPP
#define ANIMAL_SHOGI_UTILITY_EXCHANGE_HPP

#include <type_traits>
#include <utility>

namespace animal_shogi
{
    template <typename T, typename U = T>
    inline T exchange(T& obj, U&& new_value)
    {
        T old_value = std::move(obj);
        obj = std::forward<U>(new_value);
        return old_value;
    }
}

#endif