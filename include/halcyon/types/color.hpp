#pragma once

#include "SDL3/SDL_pixels.h"

#include <halcyon/internal/tags.hpp>

#include <algorithm>
#include <bit>
#include <iosfwd>

// types/color.hpp:
// An extension of SDL_Color with custom constructors,
// functions and operators.

namespace hal
{
    HAL_TAG(as_rgba);

    struct color : public SDL_Color
    {
        // A single R, G, B or A value.
        using value_t = decltype(SDL_Color::r);

        // A hex value in the form of 0xNNNNNN.
        using hex_t = std::uint32_t;

        // A byte difference between two colors.
        using diff_t = std::int16_t;

        enum : value_t
        {
            transparent = SDL_ALPHA_TRANSPARENT,
            opaque      = SDL_ALPHA_OPAQUE,
        };

        // Helper struct that represents the difference of two colors.
        struct diff
        {
            diff_t r, g, b, a;

            constexpr friend diff operator*(diff d, double mul)
            {
                return {
                    static_cast<diff_t>(d.r * mul),
                    static_cast<diff_t>(d.g * mul),
                    static_cast<diff_t>(d.b * mul),
                    static_cast<diff_t>(d.a * mul)
                };
            }
        };

        // Default constructor, creates black color.
        constexpr color()
            : color { 0x000000 }
        {
        }

        // 0xRRGGBB color, 0 - 255 alpha.
        constexpr color(hex_t rgb, value_t alpha = opaque)
            : SDL_Color {
                static_cast<value_t>((rgb >> 16) & 0xFF),
                static_cast<value_t>((rgb >> 8) & 0xFF),
                static_cast<value_t>(rgb & 0xFF),
                alpha
            }
        {
        }

        constexpr color(value_t red, value_t green, value_t blue,
            value_t alpha = opaque)
            : SDL_Color { red, green, blue, alpha }
        {
        }

        // Negate this color.
        constexpr color operator-() const
        {
            constexpr value_t max { std::numeric_limits<value_t>::max() };

            return {
                static_cast<value_t>(max - r),
                static_cast<value_t>(max - g),
                static_cast<value_t>(max - b),
                a
            };
        }

        constexpr friend bool operator==(color a, color b)
        {
            return std::bit_cast<hex_t>(a) == std::bit_cast<hex_t>(b);
        }

        constexpr friend diff operator-(color a, color b)
        {
            return {
                static_cast<diff_t>(a.r - b.r),
                static_cast<diff_t>(a.g - b.g),
                static_cast<diff_t>(a.b - b.b),
                static_cast<diff_t>(a.a - b.a)
            };
        }

        constexpr friend color operator+(color a, color b)
        {
            constexpr int max { std::numeric_limits<value_t>::max() };

            return {
                static_cast<value_t>(std::min(a.r + b.r, max)),
                static_cast<value_t>(std::min(a.g + b.g, max)),
                static_cast<value_t>(std::min(a.b + b.b, max)),
                a.a
            };
        }

        constexpr friend color operator+(color a, diff b)
        {
            return {
                static_cast<value_t>(a.r + b.r),
                static_cast<value_t>(a.g + b.g),
                static_cast<value_t>(a.b + b.b),
                static_cast<value_t>(a.a + b.a),
            };
        }

        constexpr friend color operator-(color a, diff b)
        {
            return {
                static_cast<value_t>(a.r - b.r),
                static_cast<value_t>(a.g - b.g),
                static_cast<value_t>(a.b - b.b),
                static_cast<value_t>(a.a - b.a),
            };
        }
    };

    std::ostream& operator<<(std::ostream& str, color c);

    // Predefined colors in RGB form.
    namespace palette
    {
        constexpr color::hex_t black       = 0x000000,
                               red         = 0xFF0000,
                               green       = 0x00FF00,
                               blue        = 0x0000FF,
                               yellow      = 0xFFFF00,
                               orange      = 0xFFA500,
                               cyan        = 0x00FFFF,
                               weezer_blue = 0x0099D3,
                               white       = 0xFFFFFF;

        constexpr color transparent { black, 0 };
    }
}
