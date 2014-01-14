#ifndef ANIMAL_SHOGI_AI_EVALUATION_FUNCTION_H
#define ANIMAL_SHOGI_AI_EVALUATION_FUNCTION_H

namespace animal_shogi
{
    class state;
    class piece;
    enum class turn;

    class piece_advantage
    {
    public:
        int operator()(state const& s) const;

    private:
        int to_int(piece const& p, turn current) const;
    };
}

#endif