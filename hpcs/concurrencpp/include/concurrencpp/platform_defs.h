#ifndef CONCURRENCPP_PLATFORM_DEFS_H
#define CONCURRENCPP_PLATFORM_DEFS_H

#if defined(__clang__)
#    define CRCPP_CLANG_COMPILER
#elif defined(__GNUC__) || defined(__GNUG__)
#    define CRCPP_GCC_COMPILER
#elif defined(_MSC_VER)
#    define CRCPP_MSVC_COMPILER
#endif

#if !defined(NDEBUG) || defined(_DEBUG)
#    define CRCPP_DEBUG_MODE
#endif

#include <exception>

#if defined(_LIBCPP_VERSION)
#    define CRCPP_LIBCPP_LIB
#endif

#endif  // PLATFORM_DEFS_H
