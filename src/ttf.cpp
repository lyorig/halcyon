#include <halcyon/ttf.hpp>

#include <halcyon/types/exception.hpp>
#include <halcyon/video/renderer.hpp>

#include <string_view>

using namespace hal;

text::text(hal::ref<const font> f, std::string_view str)
    : text { nullptr, f, str }
{
}

text::text(TTF_TextEngine* eng, hal::ref<const font> f, std::string_view str)
    : resource { ::TTF_CreateText(eng, f.get(), str.data(), str.length()) }
{
}

result<pixel::point> text::size() const
{
    pixel::point ret;

    return { ::TTF_GetTextSize(get(), &ret.x, &ret.y), ret };
}

font::font(accessor src, pt_t size, pass_key<ttf::context>)
    : resource { ::TTF_OpenFontIO(src.release(), true, size) }
{
}

builder::font_text font::render(c_string text) const
{
    return { *this, text, pass_key<font> {} };
}

builder::font_glyph font::render(char32_t glyph) const
{
    return { *this, glyph, pass_key<font> {} };
}

pixel_t font::height() const
{
    return static_cast<pixel_t>(::TTF_GetFontHeight(get()));
}

pixel_t font::skip() const
{
    return static_cast<pixel_t>(::TTF_GetFontLineSkip(get()));
}

const char* font::family() const
{
    return ::TTF_GetFontFamilyName(get());
}

const char* font::style() const
{
    return ::TTF_GetFontStyleName(get());
}

bool font::mono() const
{
    return ::TTF_FontIsFixedWidth(get());
}

ttf::context::context()
{
    HAL_WARN_IF(initialized(), "TTF context already exists");

    if (!::TTF_Init())
        throw exception { "TTF library initialization" };
}

ttf::context::context(bool& res)
{
    HAL_WARN_IF(initialized(), "TTF context already exists");

    res = ::TTF_Init();
}

ttf::context::~context()
{
    HAL_WARN_IF(!initialized(), "TTF context not initialized at destruction");

    ::TTF_Quit();
}

font ttf::context::load(accessor data, font::pt_t size) &
{
    return { std::move(data), size, pass_key<context> {} };
}

bool ttf::initialized()
{
    return ::TTF_WasInit() > 0;
}

using bft = builder::font_text;

bft::font_text(ref<const font> fnt, std::string_view text, pass_key<font> pk)
    : font_builder_base { fnt, pk }
    , m_text { text }
    , m_wrapLength { invalid() }
{
}

bft& bft::wrap(pixel_t wl)
{
    m_wrapLength = wl;

    return get_this();
}

surface bft::operator()(font::render_type rt)
{
    using enum font::render_type;

    if (m_wrapLength == invalid()) // Not wrapping.
    {
        switch (rt)
        {
        case solid:
            return ::TTF_RenderText_Solid(m_font->get(), m_text.data(), m_text.length(), static_cast<SDL_Color>(m_fg));

        case shaded:
            return ::TTF_RenderText_Shaded(m_font->get(), m_text.data(), m_text.length(), static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg));

        case blended:
            return ::TTF_RenderText_Blended(m_font->get(), m_text.data(), m_text.length(), static_cast<SDL_Color>(m_fg));

        case lcd:
            return ::TTF_RenderText_LCD(m_font->get(), m_text.data(), m_text.length(), static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg));
        }
    }

    else
    {
        switch (rt)
        {
        case solid:
            return ::TTF_RenderText_Solid_Wrapped(m_font->get(), m_text.data(), m_text.length(), static_cast<SDL_Color>(m_fg), m_wrapLength);

        case shaded:
            return ::TTF_RenderText_Shaded_Wrapped(m_font->get(), m_text.data(), m_text.length(), static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg), m_wrapLength);

        case blended:
            return ::TTF_RenderText_Blended_Wrapped(m_font->get(), m_text.data(), m_text.length(), static_cast<SDL_Color>(m_fg), m_wrapLength);

        case lcd:
            return ::TTF_RenderText_LCD_Wrapped(m_font->get(), m_text.data(), m_text.length(), static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg), m_wrapLength);
        };
    }

    HAL_PANIC("Unknown render type given.");
}

using bfg = builder::font_glyph;

bfg::font_glyph(ref<const font> fnt, char32_t glyph, pass_key<font> pk)
    : font_builder_base { fnt, pk }
    , m_glyph { glyph }
{
}

surface bfg::operator()(font::render_type rt)
{
    using enum font::render_type;

    switch (rt)
    {
    case solid:
        return ::TTF_RenderGlyph_Solid(m_font->get(), m_glyph, static_cast<SDL_Color>(m_fg));

    case shaded:
        return ::TTF_RenderGlyph_Shaded(m_font->get(), m_glyph, static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg));

    case blended:
        return ::TTF_RenderGlyph_Blended(m_font->get(), m_glyph, static_cast<SDL_Color>(m_fg));

    case lcd:
        return ::TTF_RenderGlyph_LCD(m_font->get(), m_glyph, static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg));
    }

    return {};
}

// ----- Text engines -----

text_engine::surface::surface()
    : engine_base { ::TTF_CreateSurfaceTextEngine() }
{
}

text_engine::renderer::renderer(ref<hal::renderer> rnd)
    : engine_base { ::TTF_CreateRendererTextEngine(rnd.get()) }
{
}
