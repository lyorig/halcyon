#pragma once

#include <halcyon/video/types.hpp>

#include <halcyon/utility/pass_key.hpp>

#include <halcyon/internal/resource.hpp>
#include <halcyon/types/result.hpp>

#include <limits>

namespace hal::detail
{
    // A base drawer class, implementing the builder method for drawing textures.
    // Designed to be used as an rvalue - all functions should only be called once.
    // It's possible to store it, but this is only recommended for:
    // a) constant textures, and
    // b) those who know what they're doing (I'm sure you do, though.)
    // "Now, now, if you follow standard insertion procedures, everything will be fine."
    template <typename T, meta::one_of<pixel_t, coord_t> Src_Type, meta::one_of<pixel_t, coord_t> Dst_Type, typename Pass, typename This>
    class drawer
    {
    protected:
        using src_t = Src_Type;
        using dst_t = Dst_Type;

        using src_point = point<src_t>;
        using src_rect  = rectangle<src_t>;

        using dst_point = point<dst_t>;
        using dst_rect  = rectangle<dst_t>;

        using this_ref = std::conditional_t<std::is_void_v<This>, drawer, This>&;

        template <meta::arithmetic U>
        consteval static U unset_pos() { return std::numeric_limits<U>::max(); }

        static pixel::point get_point(pixel::point pt)
        {
            return pt;
        }

        static pixel::point get_point(result<pixel::point> pt)
        {
            return pt.get_or({});
        }

    public:
        [[nodiscard]] drawer(ref<Pass> ths, ref<T> src)
            : m_pass { ths }
            , m_this { src }
            , m_dst { tag::as_size, get_point(src->size()) }
        {
            m_src.pos.x = unset_pos<src_t>();
        }

        // Set where to draw.
        // Discards any previous scaling and anchoring.
        [[nodiscard]] this_ref to(const dst_point& pos)
        {
            m_dst.pos = pos;
            return get_this();
        }

        // Set the destination rectangle.
        // Discards any previous scaling and anchoring.
        [[nodiscard]] this_ref to(const dst_rect& area)
        {
            m_dst = area;
            return get_this();
        }

        // Stretch across the board.
        // Do not use with scaling and anchoring.
        [[nodiscard]] this_ref to(HAL_TAG_NAME(fill))
        {
            m_dst.pos.x = unset_pos<dst_t>();
            return get_this();
        }

        // Set the source rectangle.
        // Can be called at any time.
        [[nodiscard]] this_ref from(const pixel::rect& src)
        {
            m_src      = src;
            m_dst.size = dst_point(src.size);
            return get_this();
        }

        // Set the destination's scale using a callable.
        // Call after setting the destination and before anchoring.
        template <std::invocable<dst_point> F>
        [[nodiscard]] this_ref scale(F&& scale_func)
        {
            m_dst.size = scale_func(m_dst.size);
            return get_this();
        }

        // Anchor from the destination position.
        // Call after setting the destination and scaling.
        [[nodiscard]] this_ref anchor(anchor anch)
        {
            m_dst.pos = m_dst.pos.anchor(anch, m_dst.size);
            return get_this();
        }

        // Access the source rectangle.
        src_rect& src()
        {
            return m_src;
        }

        // Get the source rectangle.
        const src_rect& src() const
        {
            return m_src;
        }

        // Access the destination rectangle.
        dst_rect& dst()
        {
            return m_dst;
        }

        // Get the destination rectangle.
        const dst_rect& dst() const
        {
            return m_dst;
        }

    protected:
        this_ref get_this()
        {
            return static_cast<this_ref>(*this);
        }

        ref<Pass> m_pass;
        ref<T>    m_this;

        dst_rect m_dst;
        src_rect m_src;
    };
}
