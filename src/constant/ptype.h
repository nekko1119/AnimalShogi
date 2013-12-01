#ifndef ANIMAL_SHOGI_CONSTANT_PTYPE_H
#define ANIMAL_SHOGI_CONSTANT_PTYPE_H

#include <stdexcept>
#include "../utility/to_string.hpp"

namespace animal_shogi
{
    enum class ptype
    {
        CHICK,      // ひよこ
        GIRAFFE,    // キリン
        ELEPHANT,   // 象
        LION,       // ライオン
        HEN,        // 鶏
    };

    template <>
    inline std::string to_string<ptype>(ptype const& value)
    {
        switch (value)
        {
        case ptype::CHICK:
            return "CH";
        case ptype::GIRAFFE:
            return "GI";
        case ptype::ELEPHANT:
            return "EL";
        case ptype::LION:
            return "LI";
        case ptype::HEN:
            return "HE";
        default:
            throw std::invalid_argument{"animal_shogi::to_string : value is invalid."};
        }
    }
}

#if BOOST_WORKAROUND(BOOST_GCC, < 409000)
#include <functional>
#include <type_traits>
namespace std
{
    template <>
    struct hash<animal_shogi::ptype>
    {
        size_t operator()(animal_shogi::ptype const& val) const
        {
            return hash<underlying_type<animal_shogi::ptype>::type>()(static_cast<underlying_type<animal_shogi::ptype>::type>(val));
        }
    };
}
#endif

#endif