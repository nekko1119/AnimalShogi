#ifndef ANIMAL_SHOGI_POINT_HPP
#define NAIMAL_SHOGI_POINT_HPP

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

        // explicitにしない
        basic_point(T val)
            : basic_point(val, val)
        {
        }

        basic_point(T x, T y)
            : x_(x), y_(y)
        {
        }

        T x() const
        {
            return x_;
        }

        void x(T val)
        {
            x_ = val;
        }

        T y() const
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

        friend bool operator==(basic_point const& lhs, basic_point const& rhs)
        {
            return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_;
        }

        friend bool operator<(basic_point const& lhs, basic_point const& rhs)
        {
            auto const cond1 = lhs.y_ < rhs.y_;
            auto const cond2 = lhs.y_ == rhs.y_ && lhs.x_ < rhs.x_;
            return cond1 || cond2;
        }

    private:
        T x_;
        T y_;
    };

    using point = basic_point<int>;
}

#endif