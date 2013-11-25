﻿#ifndef ANIMAL_SHOGI_BOARD_HPP
#define ANIMAL_SHOGI_BOARD_HPP

#include "config.hpp"
#include <array>
#include <string>
#include <boost/optional.hpp>

#include "piece.h"

namespace animal_shogi
{
    struct initial_placement_tag {};

    class board
    {
    public:
        // 動物将棋の盤のマスは横3 * 縦4。それに前後1マスずつ余白を設けたサイズ
        BOOST_STATIC_CONSTEXPR int MAX_ROW = 5;
        BOOST_STATIC_CONSTEXPR int MAX_COLUMN = 6;

        using inner_type = std::array<boost::optional<piece>, MAX_ROW>;
        using board_type = std::array<inner_type, MAX_COLUMN>;

        board() = default;
        board(initial_placement_tag);

        std::string str() const;

    private:
        board_type board_;
    };
}

#endif