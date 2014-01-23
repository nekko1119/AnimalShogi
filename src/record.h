#ifndef ANIMAL_SHOGI_RECORD_H
#define ANIMAL_SHOGI_RECORD_H

#include <string>
#include <vector>
#include <utility>
#include <boost/optional.hpp>
#include "config.hpp"
#include "piece.h"
#include "utility/point.hpp"

namespace animal_shogi
{
    // 指し手を表す構造体
    struct movement
    {
        movement(boost::optional<animal_shogi::point> from, animal_shogi::point to, piece pc) BOOST_NOEXCEPT;
        movement(movement const&) = default;
        movement(movement&& rhs) BOOST_NOEXCEPT;
        movement& operator=(movement rhs) BOOST_NOEXCEPT;
        void swap(movement& rhs) BOOST_NOEXCEPT;
        std::string str() const;

        // 元の座標
        // 持ち駒打ちの時、noneとなる
        boost::optional<animal_shogi::point> from;
        // 向かう場所
        animal_shogi::point to;
        // 駒の種類
        animal_shogi::piece pc;
    };

    inline bool operator==(movement const& left, movement const& right) BOOST_NOEXCEPT
    {
        return left.from == right.from &&
        left.to == right.to &&
        left.pc == right.pc;
    }

    void swap(movement& lhs, movement& rhs) BOOST_NOEXCEPT;

    // 棋譜を表すクラス
    // // recordクラスで棋譜自体の整合性のチェックは行わない
    class record
    {
    public:
        using container_type = std::vector<movement>;
        using value_type = container_type::value_type;
        using reference = container_type::reference;
        using const_reference = container_type::const_reference;
        using size_type = container_type::size_type;
        using iterator = container_type::iterator;
        using const_iterator = container_type::const_iterator;
        
        void push_back(movement const& mv);
        void emplace_back(boost::optional<point> from, point to, piece pc);
        void pop();
        size_type size() const BOOST_NOEXCEPT_OR_NOTHROW;
        reference top();
        const_reference top() const;
        iterator begin() BOOST_NOEXCEPT_OR_NOTHROW;
        const_iterator begin() const BOOST_NOEXCEPT_OR_NOTHROW;
        iterator end() BOOST_NOEXCEPT_OR_NOTHROW;
        const_iterator end() const BOOST_NOEXCEPT_OR_NOTHROW;
        reference operator[](size_type index);
        const_reference operator[](size_type index) const;

    private:
        container_type moves_;
    };
}

#endif