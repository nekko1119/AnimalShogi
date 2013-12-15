#ifndef ANIMAL_SHOGI_LOGGING_H
#define ANIMAL_SHOGI_LOGGING_H

#include <boost/log/trivial.hpp>
#include "../config.hpp"

#define ASHOGI_LOG_TRIVIAL(lvl)\
    BOOST_LOG_TRIVIAL(lvl) << ASHOGI_FUNC << " " << __LINE__ << " : "

namespace animal_shogi
{
    class logging
    {
    public:
        logging();
        ~logging() BOOST_NOEXCEPT_OR_NOTHROW;
    };
}

#endif