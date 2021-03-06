﻿#include "captured_piece.h"

#include <stdexcept>
#include <type_traits>
#include "utility/logging.h"

namespace animal_shogi
{
    void captured_piece::add(ptype p)
    {     
        check_on_hen(p);

        if (2 <= pieces_[static_cast<size_type>(p)])
        {
            ASHOGI_LOG_TRIVIAL(error) << "同じ持ち駒は2枚より多くなりえない";
            throw std::runtime_error("same captured piece is 2 at most");
        }
        ++pieces_[static_cast<size_type>(p)];
    }

    void captured_piece::remove(ptype p)
    {
        check_on_hen(p);

        if (pieces_[static_cast<size_type>(p)] == 0)
        {
            ASHOGI_LOG_TRIVIAL(error) << "持ち駒数が0でremoveが呼ばれた";
            throw std::runtime_error(to_string(p) + "is empty");
        }

        --pieces_[static_cast<size_type>(p)];
    }

    int captured_piece::get(ptype p) const
    {
        check_on_hen(p);

        return pieces_[static_cast<size_type>(p)];
    }

    bool captured_piece::is_empty(ptype p) const
    {
        return pieces_[static_cast<size_type>(p)] == 0;
    }

    std::string captured_piece::str() const
    {
        std::string str;
        auto const last = pieces_.size() - 1;
        for (std::size_t i = 0; i < last; ++i)
        {
            str += to_string(static_cast<ptype>(i)) + " " + std::to_string(pieces_[i]) + ", ";
        }
        str += "\n";
        return str;
    }

    void captured_piece::check_on_hen(ptype p) const
    {
        if (p == ptype::hen)
        {
            ASHOGI_LOG_TRIVIAL(error) << "ptype::henは持ち駒ではない";
            throw std::invalid_argument("ptype::hen can't be piece in hands.");
        }
    }
}