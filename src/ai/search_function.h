#ifndef ANIMAL_SHOGI_SEARCH_FUNCTION_H
#define ANIMAL_SHOGI_SEARCH_FUNCTION_H

#include <functional>
#include "../state.h"

namespace animal_shogi
{
    class minimax
    {
    public:
        using eval_func_type = std::function<int (state const&)>;

        explicit minimax(eval_func_type eval_func, std::size_t depth);

        int operator()(state const& st) const;

    private:
        using result_type = std::pair<int, boost::optional<movement>>;

        result_type execute(state const& st, std::size_t depth) const;

        eval_func_type eval_func_;
        std::size_t depth_;
    };
}

#endif