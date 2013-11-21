#ifndef ANIMAL_SHOGI_POINT_HPP
#define ANIMAL_SHOGI_POINT_HPP

#include <config.hpp>
#include <boost/operators.hpp>
#include <type_traits>

namespace animal_shogi
{
    template <typename T>
    class basic_point
        : private boost::less_than_comparable<basic_point<T>, basic_point<T>>,
        private boost::equality_comparable<basic_point<T>, basic_point<T>>,
        private boost::addable<basic_point<T>>,
        private boost::subtractable<basic_point<T>>,
        private boost::multipliable<basic_point<T>>
    {
        static_assert(std::is_arithmetic<T>::value, "basic_point<T> : T must be arithmetic type");
    public:
        using value_type = T;

        explicit basic_point(T val)
            : basic_point(val, val)
        {
        }

        basic_point(T x, T y)
            : x_(x), y_(y)
        {
        }

        T const& x() const ASHOGI_NOEXCEPT_OR_NOTHROW
        {
            return x_;
        }

        void x(T val)
        {
            x_ = val;
        }

        T const& y() const ASHOGI_NOEXCEPT_OR_NOTHROW
        {
            return y_;
        }

        void y(T val)
        {
            y_ = val;
        }

        basic_point& operator+=(basic_point const& rhs)
        {
            x_ += rhs.x_;
            y_ += rhs.y_;
            return *this;
        }

        basic_point& operator-=(basic_point const& rhs)
        {
            x_ -= rhs.x_;
            y_ -= rhs.y_;
            return *this;
        }

        basic_point& operator*=(basic_point const& rhs)
        {
            x_ *= rhs.x_;
            y_ *= rhs.y_;
            return *this;
        }

        friend bool operator==(basic_point const& lhs, basic_point const& rhs) ASHOGI_NOEXCEPT_OR_NOTHROW
        {
            return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_;
        }

        friend bool operator<(basic_point const& lhs, basic_point const& rhs) ASHOGI_NOEXCEPT_OR_NOTHROW
        {
            auto const cond1 = lhs.y_ < rhs.y_;
            auto const cond2 = lhs.y_ == rhs.y_ && lhs.x_ < rhs.x_;
            return cond1 || cond2;
        }

    private:
        T x_;
        T y_;
    };

    template <typename T>
    std::string to_string(basic_point<T> p)
    {
        return std::to_string(p.x()) + "," + std::to_string(p.y());
    }

    using point = basic_point<int>;
}

#endif