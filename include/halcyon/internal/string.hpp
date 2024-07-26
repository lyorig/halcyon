#pragma once

#include <SDL_stdinc.h>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/utility/pass_key.hpp>

namespace hal
{
    namespace proxy
    {
        class clipboard;
    }

    // SDL sometimes returns string pointers that we have to free via
    // its own function afterwards. This is a wrapper of that functionality
    // that attempts to mimic std::string as best as it can.
    class string : public detail::raii_object<char, ::SDL_free>
    {
    public:
        string(char* ptr, pass_key<proxy::clipboard>);

        const_pointer begin() const;
        pointer       begin();

        const_pointer end() const;
        pointer       end();

        std::size_t size() const;

        const_pointer data() const;
        pointer       data();

        const_pointer c_str() const;
    };

    bool operator==(string lhs, std::string_view rhs);
}
