#include <halcyon/video/driver.hpp>

#include <halcyon/video/renderer.hpp>

#include <halcyon/types/exception.hpp>

#include "SDL_video.h"

using namespace hal;

driver::index_t driver::amount()
{
    if (const int ret { ::SDL_GetNumVideoDrivers() }; ret >= 1)
        return static_cast<index_t>(ret);
    else
        return 0;
}

const char* driver::name()
{
    return ::SDL_GetCurrentVideoDriver();
}

const char* driver::name(index_t idx)
{
    return ::SDL_GetVideoDriver(idx);
}

info::sdl::renderer driver::info(index_t idx)
{
    info::sdl::renderer ret;

    if (::SDL_GetRenderDriverInfo(idx, ret.get()) != 0)
        throw hal::exception {};

    return ret;
}
