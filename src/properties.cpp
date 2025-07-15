#include <halcyon/properties.hpp>

using namespace hal;

properties::properties(SDL_PropertiesID id)
    : m_id { id }
{
}

properties::~properties()
{
    ::SDL_DestroyProperties(m_id);
}

bool properties::number_set(c_string key, std::int64_t val)
{
    return ::SDL_SetNumberProperty(m_id, key.c_str(), val);
}

std::int64_t properties::number_get(c_string key, std::int64_t default_value) const
{
    return SDL_GetNumberProperty(m_id, key.c_str(), default_value);
}

bool properties::ptr_set(c_string key, void* val)
{
    return SDL_SetPointerProperty(m_id, key.c_str(), val);
}

void* properties::ptr_get(c_string key, void* default_value) const
{
    return SDL_GetPointerProperty(m_id, key.c_str(), default_value);
}

bool properties::float_set(c_string key, float val)
{
    return ::SDL_SetFloatProperty(m_id, key.c_str(), val);
}

float properties::float_get(c_string key, float default_value) const
{
    return ::SDL_GetFloatProperty(m_id, key.c_str(), default_value);
}

bool properties::string_set(c_string key, c_string val)
{
    return ::SDL_SetStringProperty(m_id, key.c_str(), val.c_str());
}

c_string properties::string_get(c_string key, c_string default_value) const
{
    return ::SDL_GetStringProperty(m_id, key.c_str(), default_value.c_str());
}

bool properties::bool_set(c_string key, bool val)
{
    return ::SDL_SetBooleanProperty(m_id, key.c_str(), val);
}

bool properties::bool_get(c_string key, bool default_value) const
{
    return ::SDL_GetBooleanProperty(m_id, key.c_str(), default_value);
}
