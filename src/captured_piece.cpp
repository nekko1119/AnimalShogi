#include "captured_piece.h"
#include <type_traits>
#include <boost/log/trivial.hpp>

namespace animal_shogi
{
    void captured_piece::add(ptype p)
    {
        using type = piece_type::size_type;
        if (is_hen(p))
        {
            return;
        }

        if (2 <= pieces_[static_cast<type>(p)])
        {
            BOOST_LOG_TRIVIAL(error) << "captured_piece::add : 同じ持ち駒は2枚より多くなりえない";
            return;
        }
        ++pieces_[static_cast<type>(p)];
    }

    void captured_piece::remove(ptype p)
    {
        if (is_hen(p))
        {
            return;
        }

        using type = piece_type::size_type;
        if (pieces_[static_cast<type>(p)] == 0)
        {
            BOOST_LOG_TRIVIAL(error) << "captured_piece::remove : 持ち駒数が0でremoveが呼ばれた";
            return;
        }

        --pieces_[static_cast<type>(p)];
    }

    int captured_piece::get(ptype p) const
    {
        if (is_hen(p))
        {
            return 0;
        }

        using type = piece_type::size_type;
        return pieces_[static_cast<type>(p)];
    }

    std::string captured_piece::str() const
    {
        std::string str;
        auto const last = pieces_.size() - 1;
        for (decltype(last) i = 0; i < last; ++i)
        {
            str += to_string(static_cast<ptype>(i)) + " " + std::to_string(pieces_[i]) + ", ";
        }
        str += "\n";
        return str;
    }


    bool captured_piece::is_hen(ptype p) const
    {
        if (p == ptype::HEN)
        {
            BOOST_LOG_TRIVIAL(error) << "captured::get : ptype::HENは持ち駒ではない";
            return true;
        }
        return false;
    }
}