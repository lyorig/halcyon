#pragma once

#include <SDL_stdinc.h>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/utility/pass_key.hpp>

namespace hal
{
    class string;

    template <>
    class view<const string> : public detail::view_base<char>
    {
    public:
        using view_base::view_base;

        std::size_t size() const;

        const char* begin() const;
        const char* end() const;

        const char* data() const;

        const char* c_str() const;

        operator std::string_view() const;
    };

    template <>
    class view<string> : public view<const string>
    {
    private:
        using super = view<const string>;

    public:
        using super::super;

        using super::begin;
        char* begin();

        using super::end;
        char* end();

        using super::data;
        char* data();
    };

    namespace proxy
    {
        class clipboard;
    }

    // SDL sometimes returns string pointers that we have to free via
    // its own function afterwards. This is a wrapper of that functionality
    // that attempts to mimic std::string as best as it can.
    class string : public detail::raii_object<string, ::SDL_free>
    {
    public:
        using authority_t = proxy::clipboard;

        string(char* ptr, pass_key<authority_t>);
    };

    bool operator==(string lhs, std::string_view rhs);
}