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

        font() = default;

        // [private] Fonts are loaded with ttf::context::load().
        font(accessor src, pt_t size, pass_key<ttf::context>);

        // Rendering functions: text.

        [[nodiscard]] surface render_solid(std::string_view text, color fg) const;
        [[nodiscard]] surface render_solid(std::string_view text, color fg, int wrap_length) const;

        [[nodiscard]] surface render_shaded(std::string_view text, color fg, color bg) const;
        [[nodiscard]] surface render_shaded(std::string_view text, color fg, color bg, int wrap_length) const;

        [[nodiscard]] surface render_blended(std::string_view text, color fg) const;
        [[nodiscard]] surface render_blended(std::string_view text, color fg, int wrap_length) const;

        [[nodiscard]] surface render_lcd(std::string_view text, color fg, color bg) const;
        [[nodiscard]] surface render_lcd(std::string_view text, color fg, color bg, int wrap_length) const;

        // Rendering functions: glyphs.

        [[nodiscard]] surface render_solid(char32_t glyph, color fg) const;
        [[nodiscard]] surface render_shaded(char32_t glyph, color fg, color bg) const;
        [[nodiscard]] surface render_blended(char32_t glyph, color fg) const;
        [[nodiscard]] surface render_lcd(char32_t glyph, color fg, color bg) const;

        pixel_t height() const;
        pixel_t skip() const;

        const char* family() const;
        const char* style() const;

        bool mono() const;
    };

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
            context(bool& res);

            context(const context&) = delete;
            context(context&&)      = delete;

            ~context();

            // Font loading function.
            [[nodiscard]] font make_font(accessor data, font::pt_t size) const;
        };

        static_assert(std::is_empty_v<context>);

        bool initialized();
    }

    class text;

    namespace detail
    {
        template <auto Creator, auto Deleter>
        class engine_base : public resource<TTF_TextEngine, Deleter>
        {
        private:
            using super = resource<TTF_TextEngine, Deleter>;

        protected:
            engine_base() = default;
            engine_base(TTF_TextEngine* ptr)
                : super { ptr }
            {
            }

        public:
            text make_text(ref<const font> f, std::string_view text) const;
        };
    }

    namespace text_engine
    {
        // TODO
        class gpu : public detail::engine_base<::TTF_CreateGPUTextEngine, ::TTF_DestroyGPUTextEngine>
        {
        };

        class surface : public detail::engine_base<::TTF_CreateSurfaceTextEngine, ::TTF_DestroySurfaceTextEngine>
        {
        public:
            surface();
        };

        class renderer : public detail::engine_base<::TTF_CreateRendererTextEngine, ::TTF_DestroyRendererTextEngine>
        {
        public:
            renderer() = default;
            renderer(ref<hal::renderer> rnd);
        };
    }

    class text : public detail::resource<TTF_Text, ::TTF_DestroyText>
    {
    public:
        text() = default;

        text(ref<const font> f, std::string_view str);

        template <auto Creator, auto Deleter>
        text(ref<const detail::engine_base<Creator, Deleter>> gex, ref<const font> f, std::string_view str)
            : text { gex.get(), f, str }
        {
        }

        result<pixel::point> size() const;

    private:
        text(TTF_TextEngine* eng, ref<const font> f, std::string_view str);
    };
}
