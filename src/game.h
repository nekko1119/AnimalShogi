#ifndef ANIMAL_SHOGI_GAME_HPP
#define ANIMAL_SHOGI_GAME_HPP

#include <functional>
#include "state.h"

namespace animal_shogi
{
    class game
    {
    public:
        using player_type = std::function<void (state&)>;

        game(player_type black_player, player_type white_player);

        void operator()();

    private:
        state state_;
        player_type black_player_;
        player_type white_player_;
    };
}

#endif