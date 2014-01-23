#ifndef ANIMAL_SHOGI_CONSTANT_PTYPE_H
#define ANIMAL_SHOGI_CONSTANT_PTYPE_H

#include <stdexcept>
#include "../utility/to_string.hpp"

namespace animal_shogi
{
    enum class ptype
    {
        chick,      // ひよこ
        giraffe,    // キリン
        elephant,   // 象
        lion,       // ライオン
        hen,        // 鶏
    };

    template <>
    inline std::string to_string<ptype>(ptype const& value)
    {
        switch (value)
        {
        case ptype::chick:
            return "CH";
        case ptype::giraffe:
            return "GI";
        case ptype::elephant:
            return "EL";
        case ptype::lion:
            return "LI";
        case ptype::hen:
            return "HE";
        default:
            return "??";
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
            return hash<underlying_type_t<animal_shogi::ptype>>()(static_cast<underlying_type_t<animal_shogi::ptype>>(val));
        }
    };
}
#endif

#endif