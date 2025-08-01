#pragma once

#include <halcyon/types/c_string.hpp>
#include <halcyon/types/result.hpp>

#include "SDL3/SDL_properties.h"

#include <cstdint>

namespace hal
{
    // A reference to SDL-owned properties.
    class properties_ref
    {
    public:
        using id_t = SDL_PropertiesID;

    protected:
        properties_ref();
        properties_ref(id_t id);

        bool         number_set(c_string key, std::int64_t val);
        std::int64_t number_get(c_string key, std::int64_t default_value) const;

        bool  ptr_set(c_string key, void* val);
        void* ptr_get(c_string key, void* default_value) const;

        bool  float_set(c_string key, float val);
        float float_get(c_string key, float default_value) const;

        bool     string_set(c_string key, c_string val);
        c_string string_get(c_string key, c_string default_value) const;

        // These function definitions clash, so they get distinct names.
        bool bool_set(c_string key, bool val);
        bool bool_get(c_string key, bool default_value) const;

    public:
        id_t id() const;

    protected:
        id_t m_id;
    };

    // Owned properties.
    class properties : public properties_ref
    {
    public:
        properties();
        ~properties();

        properties(const properties&)            = delete;
        properties(properties&&)                 = delete;
        properties& operator=(const properties&) = delete;
        properties& operator=(properties&&)      = delete;
    };
}
