#include <halcyon/video/display.hpp>

#include <halcyon/video.hpp>

using namespace hal;

pixel::point display::size() const
{
    return { w, h };
}

pixel::format display::format() const
{
    return static_cast<pixel::format>(SDL_DisplayMode::format);
}

display::hz_t display::hz() const
{
    return static_cast<hz_t>(refresh_rate);
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
