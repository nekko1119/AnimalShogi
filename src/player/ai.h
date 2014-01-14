#ifndef ANIMAL_SHOGI_AI_H
#define ANIMAL_SHOGI_AI_H

#include <functional>

namespace animal_shogi
{
    class state;

    class ai
    {
    public:
        using search_func_type = std::function<int (state)>;

        explicit ai(search_func_type search_func);

        void operator()(state& s);

    private:
        search_func_type search_func_;
    };
}

#endif