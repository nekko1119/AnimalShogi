// C++14で入るstd::exchange関数
#ifndef ANIMAL_SHOGI_UTILITY_EXCHANGE_HPP
#define ANIMAL_SHOGI_UTILITY_EXCHANGE_HPP

#include <type_traits>
#include <utility>

namespace animal_shogi
{
    template <typename T, typename U = T>
    inline std::enable_if_t<std::is_move_assignable<T>::value, T>
        exchange(std::remove_reference_t<T>& obj, std::remove_reference_t<U>&& new_value)
    {
        T old_value = std::move(obj);
        obj = std::forward<U>(new_value);
        return old_value;
    }
}

#endif