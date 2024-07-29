#pragma once

#include <halcyon/video/types.hpp>

// internal/scaler.hpp:
// A way to scale points in-place.

namespace hal
{
    HAL_TAG(scale_width);
    HAL_TAG(scale_height);

    // A class that enables in-place scaling. This is desirable when creating
    // a surface and wanting to resize it immediately, such as:
    // font.render("Hello!").resize(hal::scale::width(128));
    class scaler
    {
    public:
        using val_t = pixel_t;
        using mul_t = f32;

        enum class type : u8
        {
            width,
            height,
            multiply
        };

        // Scale based on width.
        constexpr scaler(val_t val, HAL_TAG_NAME(scale_width))
            : m_type { type::width }
            , m_data { .val = val }
        {
        }

        // Scale based on height.
        constexpr scaler(val_t val, HAL_TAG_NAME(scale_height))
            : m_type { type::height }
            , m_data { .val = val }
        {
        }

        // Scale by multiplying both dimensions.
        constexpr scaler(mul_t mul)
            : m_type { type::multiply }
            , m_data { .mul = mul }
        {
        }

        // Get the resulting point.
        constexpr point<val_t> operator()(point<val_t> src) const
        {
            switch (m_type)
            {
                using enum type;

            case width:
                return {
                    m_data.val,
                    static_cast<val_t>(src.y * (static_cast<mul_t>(m_data.val) / src.x))
                };

            case height:
                return {
                    static_cast<val_t>(src.x * (static_cast<mul_t>(m_data.val) / src.y)),
                    m_data.val
                };

            case multiply:
                return src * m_data.mul;

            default:
                std::unreachable();
            }
        }

    private:
        type m_type;

        union
        {
            val_t val;
            mul_t mul;
        } m_data;
    };

    namespace scale
    {
        constexpr scaler width(scaler::val_t val)
        {
            return { val, tag::scale_width };
        }
        constexpr scaler height(scaler::val_t val)
        {
            return { val, tag::scale_height };
        }
        constexpr scaler mul(scaler::mul_t mul)
        {
            return { mul };
        }
    }
}
