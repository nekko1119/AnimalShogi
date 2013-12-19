#ifndef ANIMAL_SHOGI_PIECE_H
#define ANIMAL_SHOGI_PIECE_H

#include <string>
#include <vector>
#include "constant/turn.h"
#include "constant/ptype.h"
#include "utility/point.hpp"

namespace animal_shogi
{
    class piece
    {
    public:
        piece(turn turn, ptype ptype);

        std::vector<point> search(point const& current) const;
        std::string str() const;
        turn get_turn() const BOOST_NOEXCEPT_OR_NOTHROW;
        ptype get_ptype() const BOOST_NOEXCEPT_OR_NOTHROW;
        bool operator==(piece const& rhs) const BOOST_NOEXCEPT_OR_NOTHROW;
        bool operator!=(piece const& rhs) const BOOST_NOEXCEPT_OR_NOTHROW;

    private:
        turn turn_;
        ptype ptype_;
    };
}

#endif