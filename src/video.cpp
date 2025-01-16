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
    return ::SDL_HasClipboardText() == SDL_TRUE;
}

result<display> proxy::video::display_info_native(display::id_t index) const
{
    display ret;
    return { ::SDL_GetDesktopDisplayMode(index, ret.ptr(pass_key<video> {})), ret };
}

result<display> proxy::video::display_info_current(display::id_t index) const
{
    display ret;
    return { ::SDL_GetCurrentDisplayMode(index, ret.ptr(pass_key<video> {})), ret };
}
