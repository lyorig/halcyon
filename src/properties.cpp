#include <halcyon/properties.hpp>

using namespace hal;

properties_ref::properties_ref(id_t id)
    : m_id { id }
{
}

bool properties_ref::number_set(const char* key, std::int64_t val)
{
    return ::SDL_SetNumberProperty(m_id, key, val);
}

std::int64_t properties_ref::number_get(const char* key, std::int64_t default_value) const
{
    return SDL_GetNumberProperty(m_id, key, default_value);
}

bool properties_ref::ptr_set(const char* key, void* val)
{
    return SDL_SetPointerProperty(m_id, key, val);
}

void* properties_ref::ptr_get(const char* key, void* default_value) const
{
    return SDL_GetPointerProperty(m_id, key, default_value);
}

bool properties_ref::float_set(const char* key, float val)
{
    return ::SDL_SetFloatProperty(m_id, key, val);
}

float properties_ref::float_get(const char* key, float default_value) const
{
    return ::SDL_GetFloatProperty(m_id, key, default_value);
}

bool properties_ref::string_set(const char* key, const char* val)
{
    return ::SDL_SetStringProperty(m_id, key, val);
}

const char* properties_ref::string_get(const char* key, const char* default_value) const
{
    return ::SDL_GetStringProperty(m_id, key, default_value);
}

bool properties_ref::bool_set(const char* key, bool val)
{
    return ::SDL_SetBooleanProperty(m_id, key, val);
}

bool properties_ref::bool_get(const char* key, bool default_value) const
{
    return ::SDL_GetBooleanProperty(m_id, key, default_value);
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
