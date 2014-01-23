#ifndef ANIMAL_SHOGI_HUMAN_H
#define ANIMAL_SHOGI_HUMAN_H

namespace animal_shogi
{
    struct movement;
    class state;

    class human
    {
    public:
        movement const& operator()(state& s) const;
    };
}

#endif