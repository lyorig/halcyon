#include <SDL3/SDL_video.h>
#include <halcyon/video/display.hpp>

#include <halcyon/video.hpp>

using namespace hal;

display::id_t display::primary()
{
    return ::SDL_GetPrimaryDisplay();
}

display::id_t display::id() const
{
    return displayID;
}

pixel::format display::format() const
{
    return static_cast<pixel::format>(SDL_DisplayMode::format);
}

pixel::point display::size() const
{
    return { w, h };
}

float display::pixel_density() const
{
    return SDL_DisplayMode::pixel_density;
}

display::hz_t display::hz() const
{
    return static_cast<hz_t>(refresh_rate);
}

int display::hz_numerator() const
{
    return refresh_rate_numerator;
}

int display::hz_denominator() const
{
    return refresh_rate_denominator;
}

SDL_DisplayMode* display::ptr(pass_key<proxy::video>)
{
    return this;
}

std::ostream& hal::operator<<(std::ostream& str, const display& disp)
{
    return str << "[size: " << disp.size()
               << ", refresh rate:" << disp.hz()
               << " Hz, pixel format: " << to_string(disp.format())
               << ']';
}
