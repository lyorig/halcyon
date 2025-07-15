#pragma once

// library.hpp:
// Platform-dependent library routines.

#include <halcyon/types/c_string.hpp>

#include "SDL3/SDL_loadso.h"

namespace hal
{
    class library
    {
    public:
        library();
        library(c_string name);

        ~library();

        bool valid() const;

        template <typename T>
            requires std::is_function_v<T>
        T* function(c_string name)
        {
            return reinterpret_cast<T*>(::SDL_LoadFunction(m_handle, name.c_str()));
        }

    private:
        SDL_SharedObject* m_handle;
    };
}
