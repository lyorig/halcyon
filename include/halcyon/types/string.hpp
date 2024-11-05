#pragma once

#include <halcyon/internal/resource.hpp>
#include <halcyon/utility/pass_key.hpp>

#include "SDL_stdinc.h"

namespace hal
{
    namespace proxy
    {
        class video;
    }

    // SDL sometimes returns string pointers that we have to free via
    // its own function afterwards. This is a wrapper of that functionality
    // that attempts to mimic std::string as best as it can.
    class string : public detail::resource<char, ::SDL_free>
    {
    public:
        string(char* ptr, pass_key<proxy::video>);

        const_pointer begin() const;
        pointer       begin();

        const_pointer end() const;
        pointer       end();

        std::size_t size() const;

        const_pointer data() const;
        pointer       data();

        const_pointer c_str() const;
    };

    bool operator==(string lhs, const char* rhs);
}
