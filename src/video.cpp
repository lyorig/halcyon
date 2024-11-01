#include <halcyon/events.hpp>
#include <halcyon/video.hpp>

#include "SDL_clipboard.h"

using namespace hal;

proxy::video::video()
    : events { pass_key<proxy::video> {} }
{
}

string proxy::video::clipboard() const
{
    return { ::SDL_GetClipboardText(), pass_key<video> {} };
}

outcome proxy::video::clipboard(const char* text)
{
    return ::SDL_SetClipboardText(text);
}

bool proxy::video::clipboard_has_text() const
{
    return ::SDL_HasClipboardText() == SDL_TRUE;
}

outcome proxy::video::display_info_native(display& d, display::id_t index) const
{
    return ::SDL_GetDesktopDisplayMode(index, d.ptr(pass_key<video> {}));
}

outcome proxy::video::display_info_current(display& d, display::id_t index) const
{
    return ::SDL_GetCurrentDisplayMode(index, d.ptr(pass_key<video> {}));
}
