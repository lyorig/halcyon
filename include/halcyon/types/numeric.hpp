#pragma once

#include <cmath>
#include <cstdint>

#include <type_traits>

#define HAL_NO_SIZE [[no_unique_address]]

#define HAL_TAG_NAME(name) tag::name##_t

#define HAL_TAG(name)            \
    namespace tag                \
    {                            \
        struct name##_t          \
        {                        \
        } inline constexpr name; \
    }

// types.hpp:
// Various type aliases and related stuff. These are not guaranteed to be
// as large as the types they represent due to the fast type switch, so if
// you want to be certain about bit width, use the cstdint types instead.

namespace hal
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

    namespace detail
    {
        template <typename Normal_type, typename Fast_type>
        using CFT = std::conditional_t<fast_types, Fast_type, Normal_type>; // Conditional Fast Type.
    }

    // Integral types.
    using i8  = detail::CFT<std::int8_t, std::int_fast8_t>;
    using i16 = detail::CFT<std::int16_t, std::int_fast16_t>;
    using i32 = detail::CFT<std::int32_t, std::int_fast32_t>;
    using i64 = detail::CFT<std::int64_t, std::int_fast64_t>;

    using u8  = std::make_unsigned_t<i8>;
    using u16 = std::make_unsigned_t<i16>;
    using u32 = std::make_unsigned_t<i32>;
    using u64 = std::make_unsigned_t<i64>;

    // Floating-point types.
    using f32 = detail::CFT<float, std::float_t>;
    using f64 = detail::CFT<double, std::double_t>;

    template <typename Return_type, typename... Args>
    using func_ptr = Return_type (*)(Args...);

    static_assert(std::numeric_limits<f32>::is_iec559 && std::numeric_limits<f64>::is_iec559);
}