﻿#ifndef ANIMAL_SHOGI_CONDIG_HPP
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

#define ASHOGI_NO_CHAR16_T
#define ASHOGI_NO_CHAR16_T
#define ASHOGI_NO_UNICODE_LITERALS
#define ASHOGI_NO_USER_DEFINED_LITERALS
#define ASHOGI_NO_ATTRIBUTES
#define ASHOGI_NO_THREAD_LOCAL
#define ASHOGI_NO_INLINE_NAMESPACE

// バージョン確認マクロ
#if _MSC_FULL_VER == 180020827
#   define ASHOGI_MSVC12
#elif _MSC_FULL_VER == 180021114
#   define ASHOGI_MSVC12_CTP
#endif

// ヘルパー
// constexpr
#if defined(ASHOGI_NO_CONSTEXPR)
#   define ASHOGI_CONSTEXPR
#   define ASHOGI_CONSTEXPR_OR_CONST const
#else
#   define ASHOGI_CONSTEXPR constexpr
#   define ASHOGI_CONSTEXPR_OR_CONST constexpr
#endif
#define ASHOGI_STATIC_CONSTEXPR static ASHOGI_CONSTEXPR_OR_CONST

// default move function
#if defined(ASHOGI_NO_MOVE_FUNCTIONS)
#   define ASHOGI_DEFAULTED_MOVE_FUN(fun, body) fun body
#else
#   define ASHOGI_DEFAULTED_MOVE_FUN(fun, body) fun = default;
#endif

// thread safe static init
#if defined(ASHOGI_NO_THREAD_SAFE_STATIC_INIT)
#   define ASHOGI_STATIC_INIT(expr) expr
#else
#   define ASHOGI_STATIC_INIT(expr)
#endif

// noexcept
#if defined(ASHOGI_NO_NOEXCEPT)
#   define ASHOGI_NOEXCEPT
#   define ASHOGI_NOEXCEPT_OR_NOTHROW throw()
#else
#   define ASHOGI_NOEXCEPT noexcept
#   define ASHOGI_NOEXCEPT_OR_NOTHROW noexcept
#endif

// alignas
#if defined(ASHOGI_NO_ALIGNAS)
#   define ASHOGI_ALIGNAS(n)
#   define ASHOGI_ALIGNAS_OR_DECLSPEC(n) __declspec(align(n))
#else
#   define ASHOGI_ALIGNAS(n) alignas(n)
#   define ASHOGI_ALIGNAS_OR_DECLSPEC(n) alignas(n)
#endif

// decltype(auto)
#if defined(ASHOGI_NO_DECLTYPE_AUTO)
#   define ASHOGI_DECLTYPE_AUTO(expr) decltype(expr)
#else
#   define ASHOGI_DECLTYPE_AUTO(expr) decltype(auto)
#endif

// decltype(v)::type
#if defined(ASHOGI_NO_DECLTYPE_NESTEDTYPE)
#   include <type_traits>
#   define ASHOGI_DECLTYPE(expr) std::identity<decltype(expr)>::type
#else
#   define ASHOGI_DECLTYPE(expr) decltype(expr)
#endif

// Debugマクロ
#if defined(_DEBUG)
#   define ASHOGI_DEBUG
#endif

#if defined(ASHOGI_MSVC12_CTP)
#   pragma warning(disable : 4592) // constexpr関数が実行時に呼ばれているかもしれないという警告。実行時に呼ばれても何も問題が無いので消す
#endif

#pragma warning(disable : 4819) //ファイル文字コードをUnicodeに直してという警告。Boostのヘッダで出ているので無視する

#endif