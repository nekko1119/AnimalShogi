#ifndef ANIMAL_SHOGI_CONSTANT_TURN_H
#define ANIMAL_SHOGI_CONSTANT_TURN_H

#include <config.hpp>
#include <string>

namespace animal_shogi
{
    enum class turn
    {
        BLACK,  // 先手
        WHITE   // 後手
    };

    inline ASHOGI_CONSTEXPR bool is_black(turn trn) ASHOGI_NOEXCEPT_OR_NOTHROW
    {
        return trn == turn::BLACK;
    }

    inline ASHOGI_CONSTEXPR bool is_white(turn trn) ASHOGI_NOEXCEPT_OR_NOTHROW
    {
        return !is_black(trn);
    }

    inline ASHOGI_CONSTEXPR std::string get_turn_mark(turn trn) ASHOGI_NOEXCEPT_OR_NOTHROW
    {
        return is_black(trn) ? "+" : "-";
    }
}

#endif