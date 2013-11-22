#ifndef ANIMAL_SHOGI_CONDIG_HPP
#define ANIMAL_SHOGI_CONFIG_HPP

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

#if defined(BOOST_MSVC_FULL_VER)
//MSVC
#   if BOOST_MSVC < 1800
#       error "this source code can complie only Microsoft Visual C++ 12.0 or newer."
#   endif

// 2013 Nov CTP
#   if BOOST_MSVC_FULL_VER < 180021114
#       define ASHOGI_NO_THREAD_SAFE_STATIC_INIT
#       define ASHOGI_NO_DECLTYPE_AUTO
#   endif

#   if BOOST_WORKAROUND(BOOST_MSVC_FULL_VER, == 180021114)
#       pragma warning(disable : 4592) // constexpr関数が実行時に呼ばれているかもしれないという警告。実行時に呼ばれても何も問題が無いので消す
#   endif

#   pragma warning(disable : 4819) //ファイル文字コードをUnicodeに直してという警告。Boostのヘッダで出ているので無視する

#endif

// ヘルパー

// default move function
#if BOOST_WORKAROUND(BOOST_MSVC_FULL_VER, < 180021114)
#   define ASHOGI_DEFAULTED_MOVE_FUNCTION(fun, body) fun body
#else
#   define ASHOGI_DEFAULTED_MOVE_FUNCTION BOOST_DEFAULTED_FUNCTION
#endif

// thread safe static init
#if defined(ASHOGI_NO_THREAD_SAFE_STATIC_INIT)
#   define ASHOGI_STATIC_INIT(expr) expr
#else
#   define ASHOGI_STATIC_INIT(expr)
#endif

// decltype(auto)
#if defined(ASHOGI_NO_DECLTYPE_AUTO)
#   define ASHOGI_DECLTYPE_AUTO(expr) decltype(expr)
#else
#   define ASHOGI_DECLTYPE_AUTO(expr) decltype(auto)
#endif

// decltype(v)::type
#if defined(BOOST_NO_DECLTYPE_N3276)
#   include <type_traits>
#   define ASHOGI_DECLTYPE_N3276(expr) std::identity<decltype(expr)>::type
#else
#   define ASHOGI_DECLTYPE_N3276(expr) decltype(expr)
#endif

// Debugマクロ
#if defined(_DEBUG) || defined(DEBUG)
#   define ASHOGI_DEBUG
#endif

#endif