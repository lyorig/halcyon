#include <halcyon/debug.hpp>

#include "SDL_error.h"

using namespace hal;

#ifdef HAL_DEBUG_ENABLED

    #ifdef HAL_DEBUG_ADVANCED
std::ofstream debug::m_output { "Halcyon Debug Output.txt" };
const timer   debug::m_timer {};
    #endif

#endif

std::string_view debug::last_error()
{
    const char* err { ::SDL_GetError() };

    return err[0] == '\0' ? "[no SDL error]" : err;
}
