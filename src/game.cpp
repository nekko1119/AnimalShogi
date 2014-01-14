#include "game.h"

namespace animal_shogi
{
    game::game(player_type black_player, player_type white_player)
    : black_player_{std::move(black_player)}, white_player_{std::move(white_player)}
    {
    }
    
    void game::operator()()
    {
        while (!has_result(state_))
        {
            if (is_black(state_.current_turn()))
            {
                black_player_(state_);
            } else
            {
                white_player_(state_);
            }
        }
    }
}