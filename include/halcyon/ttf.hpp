#pragma once

#include <halcyon/internal/iostream.hpp>
#include <halcyon/surface.hpp>

#include <halcyon/types/c_string.hpp>

#include "SDL3_ttf/SDL_ttf.h"

// ttf.hpp:
// SDL_ttf wrappers for font loading and text rendering.

namespace hal
{
    namespace builder
    {
        class font_text;
        class font_glyph;
    }

    namespace ttf
    {
        class context;
    }

    class font : public detail::resource<TTF_Font, &::TTF_CloseFont>
    {
    public:
        using pt_t = std::uint8_t;

        enum class render_type : std::uint8_t
        {
            solid,
            shaded,
            blended,
            lcd
        };

        consteval static render_type default_render_type()
        {
            return render_type::solid;
        }

        font() = default;

        // [private] Fonts are loaded with ttf::context::load().
        font(accessor src, pt_t size, pass_key<ttf::context>);

        // Render text to a surface.
        [[nodiscard]] builder::font_text render(c_string text) const;

        // Render a single glyph to a surface.
        [[nodiscard]] builder::font_glyph render(char32_t glyph) const;

        // When sizing text, it's important to know that the vertical size
        // doesn't necessarily have to match that of the rendered surface.
        pixel::point size_text(c_string text) const;

        pixel_t height() const;
        pixel_t skip() const;

        const char* family() const;
        const char* style() const;

        bool mono() const;
    };

    constexpr c_string to_string(font::render_type rt)
    {
        switch (rt)
        {
            using enum font::render_type;

        case solid:
            return "Solid";

        case shaded:
            return "Shaded";

        case blended:
            return "Blended";

        case lcd:
            return "LCD";
        }
    }

    namespace ttf
    {
        // A class that makes sure everything TTF-related is loaded and
        // ready to use. This includes not only loading fonts, but also
        // their features - for example, font::render() will fail if a
        // TTF engine doesn't exist. TL;DR: Ensure that this object outlives
        // all fonts. This includes destructors!
        class context
        {
        public:
            // Initialize the TTF context.
            // Throws in case of failure.
            context();

            // Initialize the TTF context.
            // Writes success state to res.
            context(outcome& res);

            context(const context&) = delete;
            context(context&&)      = delete;

            ~context();

            // Font loading function.
            [[nodiscard]] font load(accessor data, font::pt_t size) &;
        };

        static_assert(std::is_empty_v<context>);

        bool initialized();
    }

    namespace detail
    {
        template <typename Derived>
        class font_builder_base
        {
        public:
            [[nodiscard]] font_builder_base(ref<const font> fnt, pass_key<font>)
                : m_font { fnt }
                , m_fg { hal::palette::white }
                , m_bg { hal::palette::transparent }
            {
            }

            // Set the foreground (text) color.
            [[nodiscard]] Derived& fg(color c)
            {
                m_fg = c;

                return get_this();
            }

            // Set the background color.
            // Does not have an effect on all render types.
            [[nodiscard]] Derived& bg(color c)
            {
                m_bg = c;

                return get_this();
            }

        protected:
            Derived& get_this()
            {
                return static_cast<Derived&>(*this);
            }

            ref<const font> m_font;

            color m_fg, m_bg;
        };
    };

    namespace builder
    {
        class font_text : public detail::font_builder_base<font_text>
        {
        public:
            [[nodiscard]] font_text(ref<const font>, c_string text, pass_key<font>);

            // How many characters to wrap this text at.
            // Zero means only wrap on newlines.
            [[nodiscard]] font_text& wrap(pixel_t wl);

            [[nodiscard]] surface operator()(font::render_type rt = font::default_render_type());

        private:
            consteval static pixel_t invalid()
            {
                return std::numeric_limits<pixel_t>::max();
            }

            c_string m_text;
            pixel_t  m_wrapLength;
        };

        class font_glyph : public detail::font_builder_base<font_glyph>
        {
        public:
            [[nodiscard]] font_glyph(ref<const font>, char32_t glyph, pass_key<font>);

            [[nodiscard]] surface operator()(font::render_type rt = font::default_render_type());

        private:
            char32_t m_glyph;
        };
    }

    // Ensure calling debug::last_error() gives accurate information.
    static_assert(::TTF_GetError == ::SDL_GetError);
}
