#include "halcyon/utility/printing.hpp"
#include <halcyon/video/window.hpp>

#include <halcyon/video.hpp>

using namespace hal;

namespace
{
    window make_fullscreen(proxy::video sys, c_string title)
    {
        window ret;

        if (const auto res = sys.display_info_native(0); res.valid())
            ret = { sys, title, res->size(), window::flag::fullscreen };

        return ret;
    }
}

window::window(proxy::video, c_string title, pixel::point size, flag_bitmask f)
    : resource { ::SDL_CreateWindow(title.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, f.mask()) }
{
}

window::window(proxy::video sys, c_string title, HAL_TAG_NAME(fullscreen))
    : window { make_fullscreen(sys, title) }
{
}

display::id_t window::display_index() const
{
    return static_cast<display::id_t>(std::max(::SDL_GetWindowDisplayIndex(get()), -1));
}

pixel::format window::pixel_format() const
{
    return static_cast<pixel::format>(::SDL_GetWindowPixelFormat(get()));
}

window::id_t window::id() const
{
    return static_cast<window::id_t>(::SDL_GetWindowID(get()));
}

window::flag_bitmask window::flags() const
{
    return static_cast<enum flag>(::SDL_GetWindowFlags(get()));
}

hal::pixel::point window::pos() const
{
    point<int> ret;

    ::SDL_GetWindowPosition(get(), &ret.x, &ret.y);

    return ret;
}

void window::pos(hal::pixel::point ps)
{
    ::SDL_SetWindowPosition(get(), ps.x, ps.y);
}

hal::pixel::point window::size() const
{
    point<int> size;

    ::SDL_GetWindowSize(get(), &size.x, &size.y);

    return size;
}

void window::size(pixel::point sz)
{
    HAL_WARN_IF(fullscreen(), "Setting size of fullscreen window - this does nothing");

    ::SDL_SetWindowSize(get(), sz.x, sz.y);
}

c_string window::title() const
{
    return ::SDL_GetWindowTitle(get());
}

void window::title(const char* val)
{
    ::SDL_SetWindowTitle(get(), val);
}

bool window::fullscreen() const
{
    return flags().any({ flag::fullscreen, flag::fullscreen_borderless });
}

outcome window::fullscreen(bool set)
{
    return ::SDL_SetWindowFullscreen(get(), set * std::to_underlying(window::flag::fullscreen));
}

ref<const renderer> window::renderer() const
{
    return { ::SDL_GetRenderer(get()), pass_key<window> {} };
}

ref<renderer> window::renderer()
{
    return { ::SDL_GetRenderer(get()), pass_key<window> {} };
}

ref<const surface> window::surface() const
{
    return { ::SDL_GetWindowSurface(get()), pass_key<window> {} };
}

std::ostream& hal::operator<<(std::ostream& str, hal::ref<hal::window> wnd)
{
    str << "[id: " << to_printable_int(wnd->id())
        << ", title: " << wnd->title()
        << ", size: " << wnd->size()
        << ", pos: " << wnd->pos()
        << ", display: " << to_printable_int(wnd->display_index())
        << ", pixel format: " << wnd->pixel_format()
        << ']';

    return str;
}
