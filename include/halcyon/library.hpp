#pragma once

// library.hpp:
// Platform-dependent library routines.

#include <halcyon/internal/resource.hpp>
#include <halcyon/types/c_string.hpp>

#include "SDL3/SDL_loadso.h"

namespace hal
{
    class library : detail::resource<SDL_SharedObject, ::SDL_UnloadObject>
    {
    public:
        library() = default;
        library(c_string name);

        template <typename T>
            requires std::is_function_v<T>
        T* function(c_string name)
        {
            return reinterpret_cast<T*>(::SDL_LoadFunction(get(), name.c_str()));
        }
    };
}
