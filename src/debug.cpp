#include <halcyon/debug.hpp>

#include "SDL3/SDL_error.h"

using namespace hal;

#ifdef HAL_DEBUG_ADVANCED
std::ofstream debug::m_output { "Halcyon Debug Output.txt" };
const timer   debug::m_timer {};
#endif

c_string debug::last_error()
{
    if (const char* const err { ::SDL_GetError() }; err[0] == '\0')
        return "[no SDL error]";
    else
        return err;
}
