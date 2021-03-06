﻿#include "ai.h"

#include "../record.h"
#include "../state.h"
#include "../utility/logging.h"

namespace animal_shogi
{
    ai::ai(search_func_type search_func)
    : search_func_{std::move(search_func)}
    {
    }

    void ai::operator()(state& s, record& r) const
    {
        auto const movable_list = enumerate_movable_pieces(s, s.current_turn());
        auto const move = search_func_(s);

        // 局面を更新
        r.push_back(movable_list[move]);
        if (movable_list[move].from)
        {
            s.update_from_board(movable_list[move].from.get(), movable_list[move].to);
        }
        else
        {
            s.update_from_cap_pc(movable_list[move].to, movable_list[move].pc);
        }
    }
}