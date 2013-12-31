#ifndef ANIMAL_SHOGI_BOARD_HPP
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
        BOOST_STATIC_CONSTEXPR int max_row = 5;
        BOOST_STATIC_CONSTEXPR int max_column = 6;

        using piece_type = boost::optional<piece>;
        using row_type = std::array<piece_type, max_row>;
        using board_type = std::array<row_type, max_column>;

        board() = default;
        board(initial_placement_tag) BOOST_NOEXCEPT_OR_NOTHROW;

        std::array<piece_type, max_row * max_column> serialize() const;
        piece_type const& at(point p) const;
        piece_type& at(point p);
        piece_type const& operator[](point p) const;
        piece_type& operator[](point p);
        std::string str() const;

    private:
        board_type board_;
    };

    bool is_within_board(point const& p);
}

#endif