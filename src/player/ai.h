#ifndef ANIMAL_SHOGI_AI_H
#define ANIMAL_SHOGI_AI_H

#include <functional>

namespace animal_shogi
{
    struct movement;
    class state;

    class ai
    {
    public:
        using search_func_type = std::function<int (state)>;

        explicit ai(search_func_type search_func);

        movement operator()(state& s) const;

    private:
        search_func_type search_func_;
    };
}

#endif