﻿#ifndef ANIMAL_SHOGI_CONSTANT_TURN_H
#define ANIMAL_SHOGI_CONSTANT_TURN_H

#include <string>
#include "../config.hpp"

namespace animal_shogi
{
    enum class turn
    {
        black,  // 先手
        white   // 後手
    };

    inline BOOST_CONSTEXPR bool is_black(turn trn) BOOST_NOEXCEPT_OR_NOTHROW
    {
        return trn == turn::black;
    }

    inline BOOST_CONSTEXPR bool is_white(turn trn) BOOST_NOEXCEPT_OR_NOTHROW
    {
        return !is_black(trn);
    }

    inline BOOST_CONSTEXPR turn operator!(turn trn) BOOST_NOEXCEPT_OR_NOTHROW
    {
        return is_black(trn) ? turn::white : turn::black;
    }

    inline std::string get_turn_mark(turn trn) BOOST_NOEXCEPT_OR_NOTHROW
    {
        return is_black(trn) ? "+" : "-";
    }
}

#endif