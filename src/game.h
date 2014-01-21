#ifndef ANIMAL_SHOGI_GAME_HPP
#define ANIMAL_SHOGI_GAME_HPP

#include <functional>
#include "state.h"

namespace animal_shogi
{
    enum class result
    {
        black,
        white,
        draw
    };

    inline BOOST_CONSTEXPR result operator!(result r)
    {
        return r == result::black ? result::white
                                : r == result::white ? result::black
                                                    : result::draw;
    }

    class game
    {
    public:
        using player_type = std::function<void (state&)>;

        game(player_type black_player, player_type white_player);

        result operator()();

    private:
        state state_;
        player_type black_player_;
        player_type white_player_;
    };
}

#endif