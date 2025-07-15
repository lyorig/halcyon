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
    // It's possible to store it, but be careful of the possible pitfalls this enables.
    // "Now, now, if you follow standard insertion procedures, everything will be fine."
    template <typename Src, typename Dst, meta::one_of<pixel_t, coord_t> Pos_Type, typename This>
    class drawer
    {
    protected:
        using pos_t = Pos_Type;

        using pos_point = point<pos_t>;
        using pos_rect  = rectangle<pos_t>;

        using this_ref = This&;

        consteval static pos_t unset_pos() { return std::numeric_limits<pos_t>::max(); }

        static pixel::point get_point(pixel::point pt)
        {
            return pt;
        }

        static pixel::point get_point(result<pixel::point> pt)
        {
            return pt.get_or({});
        }

    public:
        [[nodiscard]] drawer(ref<Dst> ths, ref<Src> src)
            : m_drawDst { ths }
            , m_drawSrc { src }
            , m_posDst { tag::as_size, get_point(src->size()) }
        {
            m_posSrc.pos.x = unset_pos();
        }

        // Set where to draw.
        // Discards any previous scaling and anchoring.
        [[nodiscard]] this_ref to(const pos_point& pos)
        {
            m_posDst.pos = pos;
            return get_this();
        }

        // Set the destination rectangle.
        // Discards any previous scaling and anchoring.
        [[nodiscard]] this_ref to(const pos_rect& area)
        {
            m_posDst = area;
            return get_this();
        }

        // Stretch across the board.
        // Do not use with scaling and anchoring.
        [[nodiscard]] this_ref to(HAL_TAG_NAME(fill))
        {
            m_posDst.pos.x = unset_pos();
            return get_this();
        }

        // Set the source rectangle.
        // Can be called at any time.
        [[nodiscard]] this_ref from(const pixel::rect& src)
        {
            m_posSrc      = src;
            m_posDst.size = pos_point(src.size);

            return get_this();
        }

        // Set the destination's scale using a callable.
        // Call after setting the destination and before anchoring.
        template <std::invocable<pos_point> F>
        [[nodiscard]] this_ref scale(F&& scale_func)
        {
            m_posDst.size = scale_func(m_posDst.size);

            return get_this();
        }

        // Anchor from the destination position.
        // Call after setting the destination and scaling.
        [[nodiscard]] this_ref anchor(anchor anch)
        {
            m_posDst.pos = m_posDst.pos.anchor(anch, m_posDst.size);

            return get_this();
        }

        // Access the source rectangle.
        pos_rect& src()
        {
            return m_posSrc;
        }

        // Get the source rectangle.
        const pos_rect& src() const
        {
            return m_posSrc;
        }

        // Access the destination rectangle.
        pos_rect& dst()
        {
            return m_posDst;
        }

        // Get the destination rectangle.
        const pos_rect& dst() const
        {
            return m_posDst;
        }

    protected:
        this_ref get_this()
        {
            return static_cast<this_ref>(*this);
        }

        ref<Dst> m_drawDst;
        ref<Src> m_drawSrc;

        pos_rect m_posDst, m_posSrc;
    };
}
