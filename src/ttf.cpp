#include <halcyon/ttf.hpp>

using namespace hal;

ttf::context::context()
{
    HAL_WARN_IF(initialized(), "TTF context already exists");

    HAL_ASSERT_VITAL(::TTF_Init() == 0, debug::last_error());

    HAL_PRINT(debug::severity::init, "Initialized TTF context");
}

ttf::context::~context()
{
    HAL_ASSERT(initialized(), "TTF context not initialized at destruction");

    ::TTF_Quit();

    HAL_PRINT("Destroyed TTF context");
}

font ttf::context::load(accessor data, u8 pt) &
{
    return { ::TTF_OpenFontRW(data.use(pass_key<context> {}), true, pt), pass_key<context> {} };
}

bool ttf::context::initialized()
{
    return ::TTF_WasInit() > 0;
}

font::font(TTF_Font* ptr, pass_key<ttf::context>)
    : raii_object { ptr }
{
    HAL_WARN_IF(height() != skip(), '\"', family(), ' ', style(), "\" has different height (", height(), "px) & skip (", skip(), "px). size_text() might not return accurate vertical results.");
}

hal::surface font::render(std::string_view text, hal::color color) const
{
    return { ::TTF_RenderUTF8_Solid_Wrapped(get(), text.data(), static_cast<SDL_Color>(color), 0), pass_key<font> {} };
}

hal::surface font::render_glyph(std::uint32_t glyph, hal::color color) const
{
    return { ::TTF_RenderGlyph32_Solid(get(), glyph, static_cast<SDL_Color>(color)), pass_key<font> {} };
}

hal::pixel_point font::size_text(const std::string_view& text) const
{
    point<int> size;

    ::TTF_SizeUTF8(get(), text.data(), &size.x, &size.y);

    return pixel_point(size);
}

hal::pixel_t font::height() const
{
    return static_cast<pixel_t>(::TTF_FontHeight(get()));
}

hal::pixel_t font::skip() const
{
    return static_cast<pixel_t>(::TTF_FontLineSkip(get()));
}

std::string_view font::family() const
{
    return ::TTF_FontFaceFamilyName(get());
}

std::string_view font::style() const
{
    return ::TTF_FontFaceStyleName(get());
}