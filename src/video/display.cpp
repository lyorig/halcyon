#include <halcyon/video/display.hpp>

#include <SDL_video.h>

#include <halcyon/debug.hpp>

using namespace hal;

display::display(index_t disp_idx, lyo::pass_key<video>)
    : m_index { disp_idx }
{
    SDL_DisplayMode dm;

    HAL_ASSERT_VITAL(::SDL_GetDesktopDisplayMode(disp_idx, &dm) == 0, debug::last_error());

    m_size.x = static_cast<pixel_t>(dm.w);
    m_size.y = static_cast<pixel_t>(dm.h);

    m_hz = static_cast<hz_t>(dm.refresh_rate);
}

pixel_point display::size() const
{
    return m_size;
}

display::hz_t display::hz() const
{
    return m_hz;
}

display::index_t display::index() const
{
    return m_index;
}

std::string_view display::name() const
{
    const char* name { ::SDL_GetDisplayName(m_index) };

    HAL_ASSERT(name != nullptr, debug::last_error());

    return name;
}