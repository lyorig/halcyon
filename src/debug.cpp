#include <halcyon/debug.hpp>

#include "SDL_error.h"

using namespace hal;

#ifdef HAL_DEBUG_ENABLED

    #ifdef HAL_DEBUG_ADVANCED
std::ofstream debug::m_output { "Halcyon Debug Output.txt" };
const timer   debug::m_timer {};
    #endif

#endif

const char* hal::last_error()
{
    if (const char* err { ::SDL_GetError() }; err[0] == '\0')
        return "[no SDL error]";
    else
        return err;
}
