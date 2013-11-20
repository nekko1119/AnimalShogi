#ifndef ANIMAL_SHOGI_CONSTANT_PTYPE_H
#define ANIMAL_SHOGI_CONSTANT_PTYPE_H

#include <utility/to_string.hpp>
#include <stdexcept>

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

#endif