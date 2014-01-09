#ifndef ANIMAL_SHOGI_GAME_HPP
#define ANIMAL_SHOGI_GAME_HPP

#include "state.h"

namespace animal_shogi
{
    template <typename BlackPlayer, typename WhitePlayer>
    class game
    {
    public:
        using black_player_type = BlackPlayer;
        using white_player_type = WhitePlayer;

        void operator()()
        {
            while (!has_result(state_))
            {
                if (is_black(state_.current_turn()))
                {
                    black_player_(state_);
                }
                else
                {
                    white_player_(state_);
                }
            }
        }

    private:
        state state_;
        black_player_type black_player_;
        white_player_type white_player_;
    };
}

#endif