#pragma once

#include <halcyon/internal/SDL_types.hpp>

#include <halcyon/types/point.hpp>
#include <iosfwd>

namespace hal
{

    template <lyo::arithmetic T>
    struct rectangle
    {
        point<T> pos, size;

        constexpr rectangle() = default;

        constexpr rectangle(T x, T y, T w, T h)
            : pos { x, y }
            , size { w, h }
        {
        }

        constexpr rectangle(const point<T>& size)
            : size { size }
        {
        }

        constexpr rectangle(const point<T>& pos, const point<T>& size)
            : pos { pos }
            , size { size }
        {
        }

        template <lyo::arithmetic Convert>
        constexpr operator rectangle<Convert>() const
        {
            return rectangle<Convert> { static_cast<point<Convert>>(pos),
                static_cast<point<Convert>>(size) };
        }

        constexpr rectangle operator*(lyo::f64 mul) const
        {
            return rectangle { pos, size * mul };
        }

        constexpr rectangle operator/(lyo::f64 div) const
        {
            return rectangle { pos, size / div };
        }

        constexpr rectangle& operator*=(lyo::f64 mul)
        {
            size *= mul;

            return *this;
        }

        constexpr rectangle& operator/=(lyo::f64 div)
        {
            size /= div;

            return *this;
        }

        constexpr operator SDL_Rect() const
        {
            using type = decltype(SDL_Rect::x);

            return SDL_Rect {
                lyo::cast<type>(pos.x),
                lyo::cast<type>(pos.y),
                lyo::cast<type>(size.x),
                lyo::cast<type>(size.y),
            };
        }

        constexpr operator SDL_FRect() const
        {
            using type = decltype(SDL_FRect::x);

            return SDL_FRect {
                lyo::cast<type>(pos.x),
                lyo::cast<type>(pos.y),
                lyo::cast<type>(size.x),
                lyo::cast<type>(size.y),
            };
        }

        constexpr SDL::rect_t<T>* addr()
            requires(lyo::is_present_v<T, SDL::pixel_t, SDL::coord_t>)
        {
            return reinterpret_cast<SDL::rect_t<T>*>(this);
        }

        constexpr const SDL::rect_t<T>* addr() const
            requires(lyo::is_present_v<T, SDL::pixel_t, SDL::coord_t>)
        {
            return reinterpret_cast<const SDL::rect_t<T>*>(this);
        }
    };

    template <lyo::arithmetic T>
    constexpr bool operator==(const rectangle<T>& r1, const rectangle<T>& r2)
    {
        return r1.pos == r2.pos && r1.size == r2.size;
    }

    template <lyo::arithmetic T>
    constexpr bool operator|(const rectangle<T>& r1, const rectangle<T>& r2)
    {
        return r1.pos.x + r1.size.x >= r2.pos.x && r1.pos.x <= r2.pos.x + r2.size.x && r1.pos.y <= r2.pos.y + r2.size.y && r1.pos.y + r1.size.y >= r2.pos.y;
    }

    template <lyo::arithmetic T>
    constexpr bool operator|(const point<T>& pt, const rectangle<T> rect)
    {
        return pt.x >= rect.pos.x && pt.x <= rect.pos.x + rect.size.x && pt.y >= rect.pos.y && pt.y <= rect.pos.y + rect.size.y;
    }

    namespace SDL
    {
        using Rect  = rectangle<SDL::pixel_t>;
        using FRect = rectangle<SDL::coord_t>;
    }
}