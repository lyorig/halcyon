#include <halcyon/video.hpp>

#include <halcyon/utility/strutil.hpp>

using namespace hal;

namespace
{
    window make_fullscreen(proxy::video sys, c_string title)
    {
        window ret;

        if (const auto res = sys.display_info_native(0))
            ret = { sys, title, res->size(), window::flag::fullscreen };

        return ret;
    }
}

window::window(proxy::video, c_string title, pixel::point size, flag_bitmask f)
    : resource { ::SDL_CreateWindow(title.data(), size.x, size.y, f.mask()) }
{
}

window::window(proxy::video sys, c_string title, HAL_TAG_NAME(fullscreen))
    : window { make_fullscreen(sys, title) }
{
}

display::id_t window::display_index() const
{
    return ::SDL_GetDisplayForWindow(get());
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

pixel::point window::min_size() const
{
    pixel::point ret;

    ::SDL_GetWindowMinimumSize(get(), &ret.x, &ret.y);

    return ret;
}

void window::min_size(pixel::point pt)
{
    ::SDL_SetWindowMinimumSize(get(), pt.x, pt.y);
}

pixel::point window::max_size() const
{
    pixel::point ret;

    ::SDL_GetWindowMaximumSize(get(), &ret.x, &ret.y);

    return ret;
}

void window::max_size(pixel::point pt)
{
    ::SDL_SetWindowMaximumSize(get(), pt.x, pt.y);
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
    return flags().any(flag::fullscreen);
}

void window::always_on_top(bool set)
{
    ::SDL_SetWindowAlwaysOnTop(get(), set);
}

bool window::fullscreen(bool set)
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
    if (!wnd->valid())
    {
        str << "[invalid]";
        return str;
    }

    str << "[id: " << to_printable_int(wnd->id())
        << ", title: " << wnd->title()
        << ", size: " << wnd->size()
        << ", pos: " << wnd->pos()
        << ", display: " << to_printable_int(wnd->display_index())
        << ", pixel format: " << wnd->pixel_format()
        << ", flags: " << int_to_hex_array(wnd->flags().mask()).data()
        << ']';

    return str;
}
