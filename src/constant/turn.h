#ifndef ANIMAL_SHOGI_CONSTANT_TURN_H
#define ANIMAL_SHOGI_CONSTANT_TURN_H

namespace animal_shogi
{
    enum class turn
    {
        BLACK,  // 先手
        WHITE   // 後手
    };

    inline bool is_black(turn trn)
    {
        return trn == turn::BLACK;
    }

    inline bool is_white(turn trn)
    {
        return !is_black(trn);
    }
}

#endif