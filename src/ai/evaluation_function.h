#ifndef ANIMAL_SHOGI_AI_EVALUATION_FUNCTION_H
#define ANIMAL_SHOGI_AI_EVALUATION_FUNCTION_H

#include <array>

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

    class random_piece_advantage
    {
    public:
        static const double max_eval_value;
        using value_type = std::array<double, 5>;
        random_piece_advantage();
        explicit random_piece_advantage(value_type values);
        int operator()(state const& s) const;
        value_type piece_values() const;

    private:
        double to_double(piece const& p, turn current) const;

        value_type piece_values_;
    };
}

#endif