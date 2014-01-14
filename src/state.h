#ifndef ANIMAL_SHOGI_STATE_H
#define ANIMAL_SHOGI_STATE_H

#include <array>
#include <cstdint>
#include <string>
#include <unordered_map>
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
        state& operator=(state rhs) BOOST_NOEXCEPT_OR_NOTHROW;

        void update_from_board(point from, point to);
        state update_from_board_copy(point from, point to) const;
        void update_from_cap_pc(point to, piece pc);
        state update_from_cap_pc_copy(point to, piece pc) const;
        std::vector<point> search(point const& pt) const;
        std::uint64_t encode() const;
        bool has_won(turn t) const;
        bool is_a_draw() const;
        turn current_turn() const BOOST_NOEXCEPT_OR_NOTHROW;

        board const& get_board() const BOOST_NOEXCEPT_OR_NOTHROW;
        captured_piece const& get_captured_piece(turn trn) const BOOST_NOEXCEPT_OR_NOTHROW;
        void swap(state& rhs) BOOST_NOEXCEPT_OR_NOTHROW;
        std::string str() const;

    private:
        using turn_type = std::underlying_type_t<turn>;

        void update_from_board_impl(point from, point to);
        void update_from_cap_pc_impl(point to, piece pc);
        void increment_state_counter();
        void reverse_turn();

        board board_;
        std::array<captured_piece, 2> captured_pieces_;
        std::unordered_map<std::uint64_t, int> state_counter_;
        record record_;
        turn current_turn_ = turn::black;
    };

    inline bool has_result(state const& st)
    {
        return st.has_won(turn::black) || st.has_won(turn::white) || st.is_a_draw();
    }

    void swap(state& lhs, state& rhs) BOOST_NOEXCEPT_OR_NOTHROW;

    // 指せる手を全て列挙する
    std::vector<movement> enumerate_movable_pieces(state const& s, turn trn);

    // あるマスに利いている駒の数を列挙する
    std::vector<movement> enumerate_control_pieces(state const& s, turn trn, point const& pos);
}

#endif