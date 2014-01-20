#ifndef ANIMAL_SHOGI_AI_EVALUATION_FUNCTION_H
#define ANIMAL_SHOGI_AI_EVALUATION_FUNCTION_H

#include <vector>
#include <unordered_map>

namespace animal_shogi
{
    class state;
    class piece;
    enum class turn;
    enum class ptype;

    class piece_advantage
    {
    public:
        using result_type = int;
        result_type operator()(state const& s) const;

    private:
        int to_int(piece const& p, turn current) const;
    };

    class random_piece_advantage
    {
    public:
        static const double max_eval_value;
        using result_type = double;
        using value_type = std::unordered_map<ptype, result_type>;

        random_piece_advantage();
        explicit random_piece_advantage(std::vector<result_type> const& values);

        result_type operator()(state const& s) const;
        std::vector<result_type> piece_values() const;

    private:
        double to_double(piece const& p, turn current) const;

        value_type piece_values_;
    };
}

#endif