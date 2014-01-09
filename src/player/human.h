#ifndef ANIMAL_SHOGI_HUMAN_H
#define ANIMAL_SHOGI_HUMAN_H

namespace animal_shogi
{
    class state;
    enum class turn;

    class human
    {
    public:
        void operator()(state& s);
    };
}

#endif