#ifndef ANIMAL_SHOGI_POINT_HPP
#define NAIMAL_SHOGI_POINT_HPP

#include <type_traits>

namespace animal_shogi
{
    template <typename T>
    struct basic_point
    {
        static_assert(std::is_arithmetic<T>::value, "basic_point<T> : T must be arithmetic type");
    public:
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

    private:
        T x_;
        T y_;
    };

    template <typename T>
    bool operator==(basic_point<T> const& lhs, basic_point<T> const& rhs)
    {
        return lhs.x() == rhs.x() && lhs.y() == rhs.y();
    }

    template <typename T>
    bool operator!=(basic_point<T> const& lhs, basic_point<T> const& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename T>
    basic_point<T> operator+(basic_point<T> lhs, basic_point<T> const& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    template <typename T>
    basic_point<T> operator-(basic_point<T> lhs, basic_point<T> const& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    using point = basic_point<int>;
}

#endif