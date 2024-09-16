#pragma once

#include <halcyon/internal/literals.hpp>

#include "SDL_endian.h"

#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>

#ifdef _MSC_VER
    #define HAL_NO_SIZE [[msvc::no_unique_address]]
#else
    #define HAL_NO_SIZE [[no_unique_address]]
#endif

// types/numeric.hpp:
// Various type aliases and related stuff. These are not guaranteed to be
// as large as the types they represent due to the fast type switch, so if
// you want to be certain about bit width, use the <cstdint> types instead.

namespace hal
{
    enum class endian : bool
    {
        lil,
        big
    };

    namespace compile_settings
    {
        // Use potentially faster types (e.g. int_fastN_t instead of intN_t).
        // I say potentially, because some platforms (for example macOS on ARM)
        // have no difference between fixed-size and fast types.
        constexpr bool fast_types {
#ifdef HAL_FAST_TYPES
            true
#else
            false
#endif
        };

        constexpr endian endianness {
#ifdef SDL_LIL_ENDIAN
            endian::lil
#elif defined SDL_BIG_ENDIAN
            endian::big
#else
    #error "No byte order specified by SDL"
#endif
        };
    }

    namespace detail
    {
        template <typename Normal_type, typename Fast_type>
        using cft = std::conditional_t<compile_settings::fast_types, Fast_type, Normal_type>; // Conditional Fast Type.
    }

    // Integral types.
    using i8  = detail::cft<std::int8_t, std::int_fast8_t>;
    using i16 = detail::cft<std::int16_t, std::int_fast16_t>;
    using i32 = detail::cft<std::int32_t, std::int_fast32_t>;
    using i64 = detail::cft<std::int64_t, std::int_fast64_t>;

    using u8  = std::make_unsigned_t<i8>;
    using u16 = std::make_unsigned_t<i16>;
    using u32 = std::make_unsigned_t<i32>;
    using u64 = std::make_unsigned_t<i64>;

    // Floating-point types.
    using f32 = detail::cft<float, std::float_t>;
    using f64 = detail::cft<double, std::double_t>;

    // Pure paranoia, but it never hurts to check stuff at compile time.
    static_assert(std::numeric_limits<f32>::is_iec559 && std::numeric_limits<f64>::is_iec559);

    namespace literals
    {
        // Signed integers
        HAL_DETAIL_DEFINE_LITERAL_OPERATOR(i8, i8);
        HAL_DETAIL_DEFINE_LITERAL_OPERATOR(i16, i16);
        HAL_DETAIL_DEFINE_LITERAL_OPERATOR(i32, i32);
        HAL_DETAIL_DEFINE_LITERAL_OPERATOR(i64, i64);

        // Unsigned integers
        HAL_DETAIL_DEFINE_LITERAL_OPERATOR(u8, u8);
        HAL_DETAIL_DEFINE_LITERAL_OPERATOR(u16, u16);
        HAL_DETAIL_DEFINE_LITERAL_OPERATOR(u32, u32);
        HAL_DETAIL_DEFINE_LITERAL_OPERATOR(u64, u64);

        // Floats
        HAL_DETAIL_DEFINE_LITERAL_OPERATOR(f32, f32);
        HAL_DETAIL_DEFINE_LITERAL_OPERATOR(f64, f64);
    }
}
