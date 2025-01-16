#pragma once

#include <halcyon/types/point.hpp>

// rectangle.hpp:
// A rectangle in 2D space. Composed of two points.

namespace hal
{
    namespace detail
    {
        template <pixel_or_coord T>
        using sdl_rect = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Rect, SDL_FRect>;
    }

    template <meta::arithmetic T>
    struct rectangle
    {
        point<T> pos, size;

        constexpr rectangle() = default;

        constexpr rectangle(T x, T y, T w, T h)
            : pos { x, y }
            , size { w, h }
        {
        }

        constexpr rectangle(const point<T>& pos)
            : pos { pos }
        {
        }

        constexpr rectangle(HAL_TAG_NAME(as_size), const point<T>& size)
            : size { size }
        {
        }

        constexpr rectangle(const point<T>& pos, const point<T>& size)
            : pos { pos }
            , size { size }
        {
        }

        constexpr bool operator==(const rectangle&) const = default;

        template <meta::arithmetic Convert>
        constexpr operator rectangle<Convert>() const
        {
            return rectangle<Convert> { static_cast<point<Convert>>(pos),
                static_cast<point<Convert>>(size) };
        }

        // Get the origin of a rectangle anchored at a certain point in this rectangle.
        constexpr point<T> anchor(anchor a, const point<T>& sz) const
        {
            point ret { pos };

            using enum hal::anchor;

            switch (a)
            {
            case center:
                ret += size / 2 - sz / 2;
                break;
            case top_left:
                // nothing to do.
                break;
            case top_right:
                ret.x += size.x - sz.x;
                break;
            case bottom_left:
                ret.y += size.y - sz.y;
                break;
            case bottom_right:
                (ret += size) -= sz;
                break;
            }

            return ret;
        }

        // Returns a SDL_(F)Rect pointer to this struct.
        constexpr auto sdl_ptr()
            requires detail::pixel_or_coord<T>
        {
            return reinterpret_cast<detail::sdl_rect<T>*>(this);
        }

        // Returns a const SDL_(F)Rect pointer to this struct.
        constexpr auto sdl_ptr() const
            requires detail::pixel_or_coord<T>
        {
            return reinterpret_cast<const detail::sdl_rect<T>*>(this);
        }

        friend std::ostream& operator<<(std::ostream& str, const rectangle& rect)
        {
            return str << '[' << rect.pos << ' ' << rect.size << ']';
        }
    };

    extern template struct rectangle<std::int8_t>;
    extern template struct rectangle<std::int16_t>;
    extern template struct rectangle<std::int32_t>;
    extern template struct rectangle<std::int64_t>;

    extern template struct rectangle<std::uint8_t>;
    extern template struct rectangle<std::uint16_t>;
    extern template struct rectangle<std::uint32_t>;
    extern template struct rectangle<std::uint64_t>;

    extern template struct rectangle<float>;
    extern template struct rectangle<double>;

    template <meta::arithmetic T>
    constexpr bool operator|(const rectangle<T>& r1, const rectangle<T>& r2)
    {
        return r1.pos.x + r1.size.x >= r2.pos.x && r1.pos.x <= r2.pos.x + r2.size.x && r1.pos.y <= r2.pos.y + r2.size.y && r1.pos.y + r1.size.y >= r2.pos.y;
    }

    template <meta::arithmetic T>
    constexpr bool operator|(const point<T>& pt, const rectangle<T> rect)
    {
        return pt.x >= rect.pos.x && pt.x <= rect.pos.x + rect.size.x && pt.y >= rect.pos.y && pt.y <= rect.pos.y + rect.size.y;
    }
}
