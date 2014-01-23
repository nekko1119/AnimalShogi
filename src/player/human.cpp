#include "human.h"

#include <iostream>
#include <limits>
#include "../record.h"
#include "../state.h"

namespace animal_shogi
{
    movement const& human::operator()(state& s) const
    {
        // 局面を描画
        std::cout << s.str() << std::endl;
        int index = 0;
        auto const movable_list = enumerate_movable_pieces(s, s.current_turn());
        std::cout << "No.\t: from\t->\tto\tpiece" << std::endl;
        for (auto const& it : movable_list)
        {
            std::cout << index << "\t: " << it.str() << std::endl;
            ++index;
        }

        // 指し手のコードを入力
        int move = -1;
        while (move < 0 || movable_list.size() - 1 < static_cast<decltype(movable_list)::size_type>(move))
        {
            std::cout << "input number >";
            std::cin >> move;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // 局面を更新
        if (movable_list[move].from)
        {
            s.update_from_board(movable_list[move].from.get(), movable_list[move].to);
        }
        else
        {
            s.update_from_cap_pc(movable_list[move].to, movable_list[move].pc);
        }

        return movable_list[move];
    }
}