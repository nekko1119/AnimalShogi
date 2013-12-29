#ifndef ANIMAL_SHOGI_CAPTURED_PIECE_H
#define ANIMAL_SHOGI_CAPTURED_PIECE_H

#include <array>
#include <string>
#include "constant/ptype.h"

namespace animal_shogi
{
    class captured_piece
    {
    public:
        // 持ち駒にできるのは象、キリン、ライオン、ひよこの４つ。ライオンも持ち駒にできるとした方が処理が楽になると思う。
        using piece_type = std::array<int, 4>;

        void add(ptype p);
        void remove(ptype p);
        int get(ptype p) const;
        bool is_empty(ptype p) const;
        std::string str() const;

    private:
        using size_type = piece_type::size_type;
        void check_on_hen(ptype p) const;

        piece_type pieces_ = {{0, 0, 0, 0}};
    };
}

#endif