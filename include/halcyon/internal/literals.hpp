#pragma once

#include <limits>

#include <halcyon/utility/metaprogramming.hpp>

#define HAL_DETAIL_DEFINE_LITERAL_OPERATOR(type, suffix)                   \
    consteval type operator""_##suffix(long double v)                      \
    {                                                                      \
        using lim = std::numeric_limits<meta::underlying_type<type>>;      \
        meta::consteval_static_assert(v >= lim::min() && v <= lim::max()); \
        return static_cast<type>(v);                                       \
    }                                                                      \
    consteval type operator""_##suffix(unsigned long long v)               \
    {                                                                      \
        return operator""_##suffix(static_cast<long double>(v));           \
    }
