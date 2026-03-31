#include <halcyon/video/driver.hpp>

#include <halcyon/video/renderer.hpp>

#include <halcyon/types/exception.hpp>

#include <SDL3/SDL_video.h>

using namespace hal;

driver::index_t driver::amount()
{
    return ::SDL_GetNumVideoDrivers();
}

const char* driver::name()
{
    return ::SDL_GetCurrentVideoDriver();
}

const char* driver::name(index_t idx)
{
    return ::SDL_GetVideoDriver(idx);
}
