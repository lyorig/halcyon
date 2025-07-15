#include <halcyon/events.hpp>
#include <halcyon/video.hpp>

#include "SDL3/SDL_clipboard.h"

using namespace hal;

proxy::video::video()
    : events { pass_key<proxy::video> {} }
{
}

string proxy::video::clipboard() const
{
    return ::SDL_GetClipboardText();
}

outcome proxy::video::clipboard(const char* text)
{
    return ::SDL_SetClipboardText(text);
}

bool proxy::video::clipboard_has_text() const
{
    return ::SDL_HasClipboardText();
}

const display* proxy::video::display_info_native(display::id_t index) const
{
    return reinterpret_cast<const display*>(::SDL_GetDesktopDisplayMode(index));
}

const display* proxy::video::display_info_current(display::id_t index) const
{
    return reinterpret_cast<const display*>(::SDL_GetCurrentDisplayMode(index));
}
