#include <halcyon/video/window.hpp>

#include <halcyon/video.hpp>

using namespace hal;

window::window(proxy::video, std::string_view title, pixel::point size, flag_bitmask f)
    : resource { ::SDL_CreateWindow(title.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, f.mask()) }
{
}

window::window(proxy::video sys, std::string_view title, HAL_TAG_NAME(fullscreen))
    : window { sys, title, sys.displays[0].size(), { flag::fullscreen } }
{
}

display::id_t window::display_index() const
{
    const auto ret = ::SDL_GetWindowDisplayIndex(get());

    HAL_ASSERT(ret >= 0, debug::last_error());

    return static_cast<display::id_t>(ret);
}

pixel::format window::pixel_format() const
{
    return static_cast<pixel::format>(::SDL_GetWindowPixelFormat(get()));
}

window::id_t window::id() const
{
    const auto ret = ::SDL_GetWindowID(get());

    HAL_ASSERT(ret != 0, debug::last_error());

    return static_cast<window::id_t>(ret);
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

std::string_view window::title() const
{
    return ::SDL_GetWindowTitle(get());
}

void window::title(std::string_view val)
{
    ::SDL_SetWindowTitle(get(), val.data());
}

bool window::fullscreen() const
{
    return flags().any({ flag::fullscreen, flag::fullscreen_borderless });
}

void window::fullscreen(bool set)
{
    HAL_ASSERT_VITAL(::SDL_SetWindowFullscreen(
                         get(),
                         set * std::to_underlying(window::flag::fullscreen))
            == 0,
        debug::last_error());
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

renderer window::make_renderer(renderer::flag_bitmask flags)
{
    return { *this, flags };
}
