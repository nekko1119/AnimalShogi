#include "record.h"
#include <stdexcept>
#include "utility/logging.h"

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
        return (from ? to_string(*from) : "----") + "\t" + to_string(to) + "\t" + pc.str();
    }

    void record::push_back(movement const& mv)
    {
        moves_.push_back(mv);
    }

    void record::push_back(movement&& mv)
    {
        moves_.push_back(std::move(mv));
    }


    void record::emplace_back(boost::optional<point> from, point to, piece pc)
    {
        moves_.emplace_back(std::move(from), std::move(to), std::move(pc));
    }

    void record::pop()
    {
        if (moves_.empty())
        {
            ASHOGI_LOG_TRIVIAL(error) << "要素が0でpopが呼ばれた";
            throw std::runtime_error("stack is empty");
        }
        moves_.pop_back();
    }

    record::size_type record::size() const BOOST_NOEXCEPT_OR_NOTHROW
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

    record::iterator record::begin() BOOST_NOEXCEPT_OR_NOTHROW
    {
        return moves_.begin();
    }

    record::const_iterator record::begin() const BOOST_NOEXCEPT_OR_NOTHROW
    {
        return moves_.begin();
    }

    record::iterator record::end() BOOST_NOEXCEPT_OR_NOTHROW
    {
        return moves_.end();
    }

    record::const_iterator record::end() const BOOST_NOEXCEPT_OR_NOTHROW
    {
        return moves_.end();
    }

    record::reference record::operator[](size_type index)
    {
        if (moves_.size() <= index)
        {
            ASHOGI_LOG_TRIVIAL(error) << "範囲外アクセス 棋譜のサイズ : " << moves_.size() << "インデックス : " << index;
            throw std::out_of_range("out of range");
        }
        return moves_[index];
    }

    record::const_reference record::operator[](size_type index) const
    {
        if (moves_.size() <= index)
        {
            ASHOGI_LOG_TRIVIAL(error) << "範囲外アクセス 棋譜のサイズ : " << moves_.size() << "インデックス : " << index;
            throw std::out_of_range("out of range");
        }
        return moves_[index];
    }
}