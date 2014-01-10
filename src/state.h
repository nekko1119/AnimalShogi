#ifndef ANIMAL_SHOGI_STATE_H
#define ANIMAL_SHOGI_STATE_H

#include <array>
#include <cstdint>
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
        std::uint64_t encode() const;
        bool has_won(turn t) const;
        turn current_turn() const BOOST_NOEXCEPT_OR_NOTHROW;

        board const& get_board() const BOOST_NOEXCEPT_OR_NOTHROW;
        captured_piece const& get_captured_piece(turn trn) const BOOST_NOEXCEPT_OR_NOTHROW;
        std::string str() const;

    private:
        using turn_type = std::underlying_type_t<turn>;

        void reverse_turn();

        board board_;
        std::array<captured_piece, 2> captured_pieces_;
        record record_;
        turn current_turn_ = turn::black;
    };

    inline bool has_result(state const& st)
    {
        return st.has_won(turn::black) || st.has_won(turn::white);
    }

    std::vector<movement> enumerate_movable_pieces(state const& s, turn trn);
}

#endif