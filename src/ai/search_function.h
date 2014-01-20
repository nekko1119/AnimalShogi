#ifndef ANIMAL_SHOGI_SEARCH_FUNCTION_H
#define ANIMAL_SHOGI_SEARCH_FUNCTION_H

#include <functional>
#include "../state.h"

namespace animal_shogi
{
    class minimax
    {
    public:
        using eval_func_type = std::function<double (state const&)>;

        minimax(eval_func_type eval_func, std::size_t depth);

        int operator()(state st);

    private:
        using result_type = std::pair<double, boost::optional<movement>>;

        result_type execute(state const& st, std::size_t depth) const;

        state state_;
        eval_func_type eval_func_;
        std::size_t depth_;
    };

    class alphabeta
    {
    public:
        using eval_func_type = std::function<double (state const&)>;

        alphabeta(eval_func_type eval_func, std::size_t depth);

        int operator()(state st);

    private:
        using result_type = std::pair<double, boost::optional<movement>>;

        result_type execute(state const& st, std::size_t depth, int alpha, int beta) const;

        state state_;
        eval_func_type eval_func_;
        std::size_t depth_;
    };
}

#endif