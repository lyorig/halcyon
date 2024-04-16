#include <halcyon/ttf.hpp>

using namespace hal;

ttf::context::context()
{
    HAL_WARN_IF(initialized(), "TTF context already exists");

    HAL_ASSERT_VITAL(::TTF_Init() == 0, debug::last_error());

    HAL_PRINT(severity::init, "Initialized TTF engine");
}

ttf::context::~context()
{
    HAL_ASSERT(initialized(), "TTF context not initialized at destruction");

    ::TTF_Quit();

    HAL_PRINT("Destroyed TTF context");
}

ttf::font ttf::context::load(accessor data, lyo::u8 pt) &
{
    return { *this, std::move(data), pt };
}

bool ttf::context::initialized()
{
    return ::TTF_WasInit() > 0;
}

ttf::font::font(context& auth, accessor data, lyo::u8 pt)
    : object { ::TTF_OpenFontRW(data.get(lyo::pass_key<ttf::font> {}), true, pt) }
{
    HAL_WARN_IF(height() != skip(), '\"', family(), ' ', style(), "\" has different height (", height(), "px) & skip (", skip(), "px). size_text() might not return accurate vertical results.");
}

ttf::font::~font()
{
    HAL_ASSERT(ttf::context::initialized(), "TTF context inactive in font destructor");
}

surface ttf::font::render(std::string_view text, hal::color color) const
{
    return { *this, text, color };
}

pixel_point ttf::font::size_text(const std::string_view& text) const
{
    point<int> size;

    ::TTF_SizeUTF8(this->ptr(), text.data(), &size.x, &size.y);

    return pixel_point(size);
}

pixel_t ttf::font::height() const
{
    return static_cast<pixel_t>(::TTF_FontHeight(this->ptr()));
}

pixel_t ttf::font::skip() const
{
    return static_cast<pixel_t>(::TTF_FontLineSkip(this->ptr()));
}

std::string_view ttf::font::family() const
{
    return ::TTF_FontFaceFamilyName(ptr());
}

std::string_view ttf::font::style() const
{
    return ::TTF_FontFaceStyleName(ptr());
}