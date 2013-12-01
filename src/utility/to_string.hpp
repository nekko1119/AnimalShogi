#ifndef ANIMAL_SHOGI_UTILITY_TO_STRING_HPP
#define ANIMAL_SHOGI_UTILITY_TO_STRING_HPP

#include <string>
#include "../config.hpp"

#if BOOST_WORKAROUND(__MINGW32__, > 0)
#include <boost/lexical_cast.hpp>

namespace std
{
    inline string to_string(int val)
    {
        return boost::lexical_cast<string>(val);
    }

    inline string to_string(unsigned val)
    {
        return boost::lexical_cast<string>(val);
    }

    inline string to_string(long val)
    {
        return boost::lexical_cast<string>(val);
    }

    inline string to_string(unsigned long val)
    {
        return boost::lexical_cast<string>(val);
    }

    inline string to_string(long long val)
    {
        return boost::lexical_cast<string>(val);
    }

    inline string to_string(unsigned long long val)
    {
        return boost::lexical_cast<string>(val);
    }

    inline string to_string(float val)
    {
        return boost::lexical_cast<string>(val);
    }

    inline string to_string(double val)
    {
        return boost::lexical_cast<string>(val);
    }

    inline string to_string(long double val)
    {
        return boost::lexical_cast<string>(val);
    }
}
#endif

namespace animal_shogi
{
    template <typename T>
    inline std::string to_string(T const& value) = delete;
}

#endif