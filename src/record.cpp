#include "record.h"
#include <boost/log/trivial.hpp>

namespace animal_shogi
{
    movement::movement(boost::optional<animal_shogi::point> from, animal_shogi::point to, piece pc) BOOST_NOEXCEPT
    : from{std::move(from)}, to{std::move(to)}, pc{std::move(pc)}
    {
    }

    movement::movement(movement&& rhs) BOOST_NOEXCEPT
    : from{std::move(rhs.from)}, to{std::move(rhs.to)}, pc{std::move(rhs.pc)}
    {
    }

    movement& movement::operator=(movement rhs) BOOST_NOEXCEPT
    {
        swap(rhs);
        return *this;
    }

    void movement::swap(movement& rhs) BOOST_NOEXCEPT
    {
        using std::swap;
        swap(from, rhs.from);
        swap(to, rhs.to);
        swap(pc, rhs.pc);
    }

    void swap(movement& lhs, movement& rhs) BOOST_NOEXCEPT
    {
        lhs.swap(rhs);
    }

    std::string movement::str() const
    {
        return (from ? to_string(*from) : "none") + "\t" + to_string(to) + "\t" + pc.str();
    }

    void record::push(boost::optional<point> from, point to, piece pc)
    {
        moves_.emplace_back(std::move(from), std::move(to), std::move(pc));
    }

    void record::pop()
    {
        if (moves_.empty())
        {
            BOOST_LOG_TRIVIAL(error) << "record::pop : 要素が0でpopが呼ばれた";
            return;
        }
        moves_.pop_back();
    }

    record::size_type record::size() const
    {
        return moves_.size();
    }

    record::reference record::top()
    {
        return moves_.back();
    }

    record::const_reference record::top() const
    {
        return moves_.back();
    }

    record::iterator record::begin()
    {
        return moves_.begin();
    }

    record::const_iterator record::begin() const
    {
        return moves_.begin();
    }

    record::iterator record::end()
    {
        return moves_.end();
    }

    record::const_iterator record::end() const
    {
        return moves_.end();
    }

    record::reference record::operator[](size_type index)
    {
        if (moves_.size() <= index)
        {
            BOOST_LOG_TRIVIAL(error) << "record::operator[] : 範囲外アクセス 棋譜のサイズ : " << moves_.size() << "インデックス : " << index;
        }
        return moves_[index];
    }

    record::const_reference record::operator[](size_type index) const
    {
        if (moves_.size() <= index)
        {
            BOOST_LOG_TRIVIAL(error) << "record::operator[] : 範囲外アクセス 棋譜のサイズ : " << moves_.size() << "インデックス : " << index;
        }
        return moves_[index];
    }
}