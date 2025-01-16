#include <halcyon/library.hpp>

#include "SDL3/SDL_loadso.h"

using namespace hal;

library::library()
    : m_handle { nullptr }
{
}

library::library(c_string name)
    : m_handle { ::SDL_LoadObject(name.c_str()) }
{
}

library::~library()
{
    ::SDL_UnloadObject(m_handle);
}

bool library::valid() const
{
    return m_handle != nullptr;
}

void* library::base_function(c_string s) const
{
    return ::SDL_LoadFunction(m_handle, s.c_str());
}
