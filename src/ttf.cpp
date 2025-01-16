#include <halcyon/ttf.hpp>

#include <halcyon/types/exception.hpp>

using namespace hal;

namespace
{
    outcome ttf_init()
    {
        return ::TTF_Init();
    }
}

font::font(accessor src, pt_t size, pass_key<ttf::context>)
    : resource { ::TTF_OpenFontRW(src.use(pass_key<font> {}), true, size) }
{
    HAL_WARN_IF(height() != skip(), '\"', family(), ' ', style(), "\" has different height (", height(), "px) & skip (", skip(), "px). size_text() might not return accurate vertical results.");
}

builder::font_text font::render(c_string text) const
{
    return { *this, text, pass_key<font> {} };
}

builder::font_glyph font::render(char32_t glyph) const
{
    return { *this, glyph, pass_key<font> {} };
}

pixel::point font::size_text(c_string text) const
{
    point<int> size;

    ::TTF_SizeUTF8(get(), text.data(), &size.x, &size.y);

    return pixel::point(size);
}

pixel_t font::height() const
{
    return static_cast<pixel_t>(::TTF_FontHeight(get()));
}

pixel_t font::skip() const
{
    return static_cast<pixel_t>(::TTF_FontLineSkip(get()));
}

const char* font::family() const
{
    return ::TTF_FontFaceFamilyName(get());
}

const char* font::style() const
{
    return ::TTF_FontFaceStyleName(get());
}

bool font::mono() const
{
    return ::TTF_FontFaceIsFixedWidth(get());
}

ttf::context::context()
{
    HAL_WARN_IF(initialized(), "TTF context already exists");

    if (!ttf_init())
        throw exception { "TTF library initialization" };
}

ttf::context::context(outcome& res)
{
    HAL_WARN_IF(initialized(), "TTF context already exists");

    res = ttf_init();
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

bft::font_text(ref<const font> fnt, c_string text, pass_key<font> pk)
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
            return { ::TTF_RenderUTF8_Solid(m_font->get(), m_text.data(), static_cast<SDL_Color>(m_fg)), pass_key<font_text> {} };

        case shaded:
            return { ::TTF_RenderUTF8_Shaded(m_font->get(), m_text.data(), static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg)), pass_key<font_text> {} };

        case blended:
            return { ::TTF_RenderUTF8_Blended(m_font->get(), m_text.data(), static_cast<SDL_Color>(m_fg)), pass_key<font_text> {} };

        case lcd:
            return { ::TTF_RenderUTF8_LCD(m_font->get(), m_text.data(), static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg)), pass_key<font_text> {} };
        }
    }

    else
    {
        switch (rt)
        {
        case solid:
            return { ::TTF_RenderUTF8_Solid_Wrapped(m_font->get(), m_text.data(), static_cast<SDL_Color>(m_fg), m_wrapLength), pass_key<font_text> {} };

        case shaded:
            return { ::TTF_RenderUTF8_Shaded_Wrapped(m_font->get(), m_text.data(), static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg), m_wrapLength), pass_key<font_text> {} };

        case blended:
            return { ::TTF_RenderUTF8_Blended_Wrapped(m_font->get(), m_text.data(), static_cast<SDL_Color>(m_fg), m_wrapLength), pass_key<font_text> {} };

        case lcd:
            return { ::TTF_RenderUTF8_LCD_Wrapped(m_font->get(), m_text.data(), static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg), m_wrapLength), pass_key<font_text> {} };
        }
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
        return { ::TTF_RenderGlyph32_Solid(m_font->get(), m_glyph, static_cast<SDL_Color>(m_fg)), pass_key<font_glyph> {} };

    case shaded:
        return { ::TTF_RenderGlyph32_Shaded(m_font->get(), m_glyph, static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg)), pass_key<font_glyph> {} };

    case blended:
        return { ::TTF_RenderGlyph32_Blended(m_font->get(), m_glyph, static_cast<SDL_Color>(m_fg)), pass_key<font_glyph> {} };

    case lcd:
        return { ::TTF_RenderGlyph32_LCD(m_font->get(), m_glyph, static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg)), pass_key<font_glyph> {} };
    }

    return {};
}
