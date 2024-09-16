#include <halcyon/events.hpp>
#include <halcyon/video.hpp>

using namespace hal;

proxy::clipboard::clipboard(pass_key<video>)
{
}

proxy::display::display(pass_key<video>)
{
}

window proxy::video::make_window(const char* title, pixel::point size, window::flag_bitmask flags) const&
{
    return { *this, title, size, flags };
}

window proxy::video::make_window(const char* title, HAL_TAG_NAME(fullscreen)) const&
{
    return { *this, title, tag::fullscreen };
}

string proxy::clipboard::text() const
{
    char* text { ::SDL_GetClipboardText() };

    HAL_ASSERT(text[0] != '\0' || has_text(), debug::last_error());

    return { text, pass_key<proxy::clipboard> {} };
};

bool proxy::clipboard::text(const char* what)
{
    return ::SDL_SetClipboardText(what) == 0;
}

bool proxy::clipboard::has_text() const
{
    return ::SDL_HasClipboardText() == SDL_TRUE;
}

display::id_t proxy::display::size() const
{
    if (const int ret { ::SDL_GetNumVideoDisplays() }; ret >= 1)
        return static_cast<hal::display::id_t>(ret);
    else
        return 0;
}

const char* proxy::display::name(hal::display::id_t id) const
{
    return ::SDL_GetDisplayName(id);
}

info::sdl::display proxy::display::operator[](hal::display::id_t idx) const
{
    return { idx, pass_key<proxy::display> {} };
}

proxy::video::video()
    : events { pass_key<proxy::video> {} }
    , clipboard { pass_key<proxy::video> {} }
    , displays { pass_key<proxy::video> {} }
{
}
