#pragma once

#include <halcyon/types/result.hpp>

#include <SDL3/SDL_properties.h>

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

        bool                       number_set(const char* key, std::int64_t val);
        [[nodiscard]] std::int64_t number_get(const char* key, std::int64_t default_value) const;

        bool                ptr_set(const char* key, void* val);
        [[nodiscard]] void* ptr_get(const char* key, void* default_value) const;

        bool                float_set(const char* key, float val);
        [[nodiscard]] float float_get(const char* key, float default_value) const;

        bool                      string_set(const char* key, const char* val);
        [[nodiscard]] const char* string_get(const char* key, const char* default_value) const;

        // These function definitions clash, so they get distinct names.
        bool               bool_set(const char* key, bool val);
        [[nodiscard]] bool bool_get(const char* key, bool default_value) const;

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
