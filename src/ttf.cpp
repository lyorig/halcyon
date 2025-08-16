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

surface font::render_solid(std::string_view text, color fg) const
{
    return ::TTF_RenderText_Solid(get(), text.data(), text.length(), fg);
}

surface font::render_solid(std::string_view text, color fg, int wrap_length) const
{
    return ::TTF_RenderText_Solid_Wrapped(get(), text.data(), text.length(), fg, wrap_length);
}

surface font::render_shaded(std::string_view text, color fg, color bg) const
{
    return ::TTF_RenderText_Shaded(get(), text.data(), text.length(), fg, bg);
}

surface font::render_shaded(std::string_view text, color fg, color bg, int wrap_length) const
{
    return ::TTF_RenderText_Shaded_Wrapped(get(), text.data(), text.length(), fg, bg, wrap_length);
}

surface font::render_blended(std::string_view text, color fg) const
{
    return ::TTF_RenderText_Blended(get(), text.data(), text.length(), fg);
}

surface font::render_blended(std::string_view text, color fg, int wrap_length) const
{
    return ::TTF_RenderText_Blended_Wrapped(get(), text.data(), text.length(), fg, wrap_length);
}

surface font::render_lcd(std::string_view text, color fg, color bg) const
{
    return ::TTF_RenderText_LCD(get(), text.data(), text.length(), fg, bg);
}

surface font::render_lcd(std::string_view text, color fg, color bg, int wrap_length) const
{
    return ::TTF_RenderText_LCD_Wrapped(get(), text.data(), text.length(), fg, bg, wrap_length);
}

surface font::render_solid(char32_t glyph, color fg) const
{
    return ::TTF_RenderGlyph_Solid(get(), glyph, fg);
}

surface font::render_shaded(char32_t glyph, color fg, color bg) const
{
    return ::TTF_RenderGlyph_Shaded(get(), glyph, fg, bg);
}

surface font::render_blended(char32_t glyph, color fg) const
{
    return ::TTF_RenderGlyph_Blended(get(), glyph, fg);
}

surface font::render_lcd(char32_t glyph, color fg, color bg) const
{
    return ::TTF_RenderGlyph_LCD(get(), glyph, fg, bg);
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

font ttf::context::make_font(accessor data, font::pt_t size) const
{
    return { std::move(data), size, pass_key<context> {} };
}

bool ttf::initialized()
{
    return ::TTF_WasInit() != 0;
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
