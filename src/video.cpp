#include <halcyon/events.hpp>
#include <halcyon/video.hpp>

using namespace hal;

proxy::clipboard::clipboard(pass_key<video>)
{
}

proxy::display::display(pass_key<video>)
{
}

window proxy::video::make_window(std::string_view title, pixel::point size, window::flag_bitmask flags) const&
{
    return { *this, title, size, flags };
}

window proxy::video::make_window(std::string_view title, HAL_TAG_NAME(fullscreen)) const&
{
    return { *this, title, tag::fullscreen };
}

string proxy::clipboard::text() const
{
    char* text { ::SDL_GetClipboardText() };

    HAL_ASSERT(text[0] != '\0' || has_text(), debug::last_error());

    return { text, pass_key<proxy::clipboard> {} };
};

void proxy::clipboard::text(std::string_view what)
{
    HAL_ASSERT_VITAL(::SDL_SetClipboardText(what.data()) == 0, debug::last_error());
}

bool proxy::clipboard::has_text() const
{
    return ::SDL_HasClipboardText() == SDL_TRUE;
}

display::id_t proxy::display::size() const
{
    const auto ret = ::SDL_GetNumVideoDisplays();

    HAL_ASSERT(ret >= 1, debug::last_error());

    return static_cast<hal::display::id_t>(ret);
}

std::string_view proxy::display::name(hal::display::id_t id) const
{
    const char* ret { ::SDL_GetDisplayName(id) };

    HAL_ASSERT(ret != nullptr, debug::last_error());

    return ret;
}

info::sdl::display proxy::display::operator[](hal::display::id_t idx) const
{
    return { idx, pass_key<proxy::display> {} };
}

proxy::video::subsystem(pass_key<hal::system::video>)
    : subsystem {}

{
}

proxy::video::subsystem()
    : events { pass_key<proxy::video> {} }
    , clipboard { pass_key<proxy::video> {} }
    , displays { pass_key<proxy::video> {} }
{
}
