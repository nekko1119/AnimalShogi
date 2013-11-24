#ifndef ANIMAL_SHOGI_PIECE_H
#define ANIMAL_SHOGI_PIECE_H

#include <constant/turn.h>
#include <constant/ptype.h>
#include <utility/point.hpp>
#include <string>
#include <vector>

namespace animal_shogi
{
    class piece
    {
    public:
        piece(turn turn, ptype ptype);

        std::vector<point> calc_moves(point const& current) const;
        std::string str() const;
        bool operator==(piece const& rhs) const;
        bool operator!=(piece const& rhs) const;

    private:
        turn turn_;
        ptype ptype_;
    };
}

#endif