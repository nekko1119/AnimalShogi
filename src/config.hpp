#ifndef ANIMAL_SHOGI_CONDIG_HPP
#define ANIMAL_SHOGI_CONFIG_HPP

// MSVC かつ Visual C++ 12.0 以上(Visual Studio 2013 以上)
#if !defined(_MSC_VER) || _MSC_VER < 1800
#   error "this source codes can complie only Microsoft Visual C++ 12.0 or newer."
#endif

// 2013 Nov CTP
#if _MSC_FULL_VER < 180021114
// C++11
#   define ASHOGI_NO_CONSTEXPR
#   define ASHOGI_NO_MOVE_FUNCTIONS
#   define ASHOGI_NO_REF_QUALIFIERS
#   define ASHOGI_NO_THREAD_SAFE_STATIC_INIT
#   define ASHOGI_NO_NOEXCEPT
#   define ASHOGI_NO_ALIGNAS
#   define ASHOGI_NO_ALIGNOF
#   define ASHOGI_NO_EXTENDED_SIZEOF
#   define ASHOGI_NO_INHERITING_CONSTRUCTOR
#   define ASHOGI_NO_DECLTYPE_NESTEDTYPE
// C++14
#   define ASHOGI_NO_DECLTYPE_AUTO
#   define ASHOGI_NO_GENERIC_LAMBDAS
#   define ASHOGI_NO_AUTO_FUNCTION
#endif

// ヘルパー
// constexpr
#if defined(ASHOGI_NO_CONSTEXPR)
#   define ASHOGI_CONSTEXPR
#   define ASHOGI_CONSTEXPR_OR_CONST const
#elif
#   define ASHOGI_CONSTEXPR constexpr
#   define ASHOGI_CONSTEXPR_OR_CONST constexpr
#endif
#define ASHOGI_STATIC_CONSTEXPR static ASHOGI_CONSTEXPR_OR_CONST

// default move function
#if defined(ASHOGI_NO_MOVE_FUNCTIONS)
#   define ASHOGI_DEFAULTED_MOVE_FUN(fun, body) fun body
#elif
#   define ASHOGI_DEFAULTED_MOVE_FUN(fun, body) fun = default;
#endif

// thread safe static init
#if defined(ASHOGI_NO_THREAD_SAFE_STATIC_INIT)
#   define ASHOGI_STATIC_INIT(expr) expr
#elif
#   define ASHOGI_STATIC_INIT(expr)
#endif

// noexcept
#if defined(ASHOGI_NO_NOEXCEPT)
#   define ASHOGI_NOEXCEPT
#   define ASHOGI_NOEXCEPT_OR_NOTHROW throw()
#elif
#   define ASHOGI_NOEXCEPT noexcept
#   define ASHOGI_NOEXCEPT_OR_NOTHROW noexcept
#endif

// alignas
#if defined(ASHOGI_NO_ALIGNAS)
#   define ASHOGI_ALIGNAS(n)
#   define ASHOGI_ALIGNAS_OR_DECLSPEC(n) __declspec(align(n))
#elif
#   define ASHOGI_ALIGNAS(n) alignas(n)
#   define ASHOGI_ALIGNAS_OR_DECLSPEC(n) alignas(n)
#endif

// decltype(auto)
#if defined(ASHOGI_NO_DECLTYPE_AUTO)
#   define ASHOGI_DECLTYPE_AUTO(expr) decltype(expr)
#elif
#   define ASHOGI_DECLTYPE_AUTO(expr) decltype(auto)
#endif

// decltype(v)::type
#if defined(ASHOGI_NO_DECLTYPE_NESTEDTYPE)
#   include <type_traits>
#   define ASHOGI_DECLTYPE(expr) std::identity<decltype(expr)>::type
#elif
#   define ASHOGI_DECLTYPE(expr) decltype(expr)
#endif

// Debugマクロ
#if defined(_DEBUG)
#   define ASHOGI_DEBUG
#endif

#endif