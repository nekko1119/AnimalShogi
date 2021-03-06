﻿#ifndef ANIMAL_SHOGI_SEARCH_FUNCTION_H
#define ANIMAL_SHOGI_SEARCH_FUNCTION_H

#include <functional>
#include <map>
#include "../state.h"

namespace animal_shogi
{
    class minimax
    {
    public:
        using eval_func_type = std::function<double (state const&, turn)>;

        minimax(eval_func_type eval_func, std::size_t depth);

        int operator()(state st) const;

    private:
        using result_type = std::pair<double, boost::optional<movement>>;

        result_type execute(state const& st, std::size_t depth, std::multimap<double, movement>& move_evals) const;

        eval_func_type eval_func_;
        std::size_t depth_;
    };

    class alphabeta
    {
    public:
        using eval_func_type = std::function<double (state const&, turn)>;

        alphabeta(eval_func_type eval_func, std::size_t depth);

        int operator()(state st) const;

    private:
        using result_type = double;

        result_type execute(state const& st, turn trn, movement const& move, std::size_t depth, double alpha, double beta) const;

        eval_func_type eval_func_;
        std::size_t depth_;
    };
}

#endif