#ifndef ANIMAL_SHOGI_SINGLETON_HOLDER_HPP
#define ANIMAL_SHOGI_SINGLETON_HOLDER_HPP

#include <boost/noncopyable.hpp>

namespace animal_shogi
{
    template <typename T>
    class singleton_holder
        : private boost::noncopyable
    {
    public:
        using value_type = T;

        // シングルトンはムーブも不可とする
        singleton_holder(singleton_holder&&) = delete;
        singleton_holder& operator=(singleton_holder&&) = delete;

        static T& get()
        {
            static T instance;
            return instance;
        }
    };
}

#endif