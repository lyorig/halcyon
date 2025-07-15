#include <halcyon/properties.hpp>

using namespace hal;

properties_ref::properties_ref(id_t id)
    : m_id { id }
{
}

bool properties_ref::number_set(c_string key, std::int64_t val)
{
    return ::SDL_SetNumberProperty(m_id, key.c_str(), val);
}

std::int64_t properties_ref::number_get(c_string key, std::int64_t default_value) const
{
    return SDL_GetNumberProperty(m_id, key.c_str(), default_value);
}

bool properties_ref::ptr_set(c_string key, void* val)
{
    return SDL_SetPointerProperty(m_id, key.c_str(), val);
}

void* properties_ref::ptr_get(c_string key, void* default_value) const
{
    return SDL_GetPointerProperty(m_id, key.c_str(), default_value);
}

bool properties_ref::float_set(c_string key, float val)
{
    return ::SDL_SetFloatProperty(m_id, key.c_str(), val);
}

float properties_ref::float_get(c_string key, float default_value) const
{
    return ::SDL_GetFloatProperty(m_id, key.c_str(), default_value);
}

bool properties_ref::string_set(c_string key, c_string val)
{
    return ::SDL_SetStringProperty(m_id, key.c_str(), val.c_str());
}

c_string properties_ref::string_get(c_string key, c_string default_value) const
{
    return ::SDL_GetStringProperty(m_id, key.c_str(), default_value.c_str());
}

bool properties_ref::bool_set(c_string key, bool val)
{
    return ::SDL_SetBooleanProperty(m_id, key.c_str(), val);
}

bool properties_ref::bool_get(c_string key, bool default_value) const
{
    return ::SDL_GetBooleanProperty(m_id, key.c_str(), default_value);
}

properties_ref::id_t properties_ref::id() const
{
    return m_id;
}

properties::properties()
    : properties_ref { ::SDL_CreateProperties() }
{
}

properties::~properties()
{
    ::SDL_DestroyProperties(m_id);
}
