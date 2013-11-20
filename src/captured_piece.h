#ifndef ANIMAL_SHOGI_CAPTURED_PIECE_H
#define ANIMAL_SHOGI_CAPTURED_PIECE_H

#include <constant/ptype.h>
#include <array>
#include <string>

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
        std::string str() const;

    private:
        bool is_hen(ptype p) const;

        piece_type pieces_ = {{0, 0, 0, 0}};
    };
}

#endif