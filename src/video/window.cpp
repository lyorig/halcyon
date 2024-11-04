#include <halcyon/video/window.hpp>

#include <halcyon/video.hpp>

using namespace hal;

namespace
{
    pixel::point display_size(proxy::video v, display::id_t index)
    {
        display d;

        HAL_WARN_IF_VITAL(!v.display_info_native(d, index), "Could not get display info: ", last_error());

        return d.size();
    }
}

window::window(proxy::video, c_string title, pixel::point size, flag_bitmask f)
    : resource { ::SDL_CreateWindow(title.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, f.mask()) }
{
}

window::window(proxy::video sys, c_string title, HAL_TAG_NAME(fullscreen))
    : window { sys, title, display_size(sys, 0), { flag::fullscreen } }
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

void window::size(scaler scl)
{
    size(scl(size()));
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
