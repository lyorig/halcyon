#include <halcyon/video/driver.hpp>

#include <halcyon/video/renderer.hpp>

#include <halcyon/types/exception.hpp>

#include "SDL3/SDL_video.h"

using namespace hal;

driver::index_t driver::amount()
{
    if (const int ret { ::SDL_GetNumVideoDrivers() }; ret >= 1)
        return static_cast<index_t>(ret);
    else
        return 0;
}

c_string driver::name()
{
    return ::SDL_GetCurrentVideoDriver();
}

c_string driver::name(index_t idx)
{
    return ::SDL_GetVideoDriver(idx);
}

outcome driver::info(renderer_info& info, index_t idx)
{
    return ::SDL_GetRenderDriverInfo(idx, &info);
}
