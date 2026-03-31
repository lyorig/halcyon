#pragma once

// library.hpp:
// Platform-dependent library routines.

#include <halcyon/internal/resource.hpp>

#include <SDL3/SDL_loadso.h>

namespace hal
{
    class library : detail::resource<SDL_SharedObject, ::SDL_UnloadObject>
    {
    public:
        library() = default;

        library(const char* name);
        library(std::nullptr_t) = delete;

        template <typename T>
            requires std::is_function_v<T>
        T* function(const char* name)
        {
            return reinterpret_cast<T*>(::SDL_LoadFunction(get(), name));
        }
    };
}
