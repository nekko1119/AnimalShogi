#ifndef ANIMAL_SHOGI_PIECE_H
#define ANIMAL_SHOGI_PIECE_H

#include "point.hpp"
#include <boost/noncopyable.hpp>

namespace animal_shogi
{
    class piece
        : private boost::noncopyable
    {
    public:

    private:
        point point_;   //現在の座標(TODO持ち駒のときに座標をどう表すか)
    };
}

#endif