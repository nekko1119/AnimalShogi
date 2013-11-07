#ifndef ANIMAL_SHOGI_PIECE_H
#define ANIMAL_SHOGI_PIECE_H

#include "constant/player.h"
#include "constant/ptype.h"
#include "point.hpp"
#include <vector>

namespace animal_shogi
{
    class piece
    {
    public:
        piece(player player, ptype ptype);

        std::vector<point> calc_moves(point const& current) const;

    private:
        player player_;
        ptype ptype_;
    };
}

#endif