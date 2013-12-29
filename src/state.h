#ifndef ANIMAL_SHOGI_STATE_H
#define ANIMAL_SHOGI_STATE_H

#include <array>
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include "board.h"
#include "captured_piece.h"
#include "piece.h"
#include "record.h"

namespace animal_shogi
{
    class state
    {
    public:
        state() BOOST_NOEXCEPT;

        void update_from_board(point from, point to);
        void update_from_cap_pc(point to, piece pc);
        std::vector<point> search(point const& pt) const;

        board const& get_board() const BOOST_NOEXCEPT_OR_NOTHROW;
        captured_piece const& get_captured_piece(turn trn) const BOOST_NOEXCEPT_OR_NOTHROW;
        std::string str() const;

    private:
        using turn_type = std::underlying_type_t<turn>;

        void drop_piece(point from, piece pc);

        board board_;
        std::array<captured_piece, 2> captured_pieces_;
        record record_;
    };
}

#endif