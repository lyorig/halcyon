#pragma once

// utility/pass_key.hpp:
// Implementation of a C++ idiom to enable class construction from
// specific sources. As you can see, it's not that complicated.

namespace hal
{
    template <typename T>
    class pass_key
    {
    private:
        friend T;

        constexpr pass_key() { }
    };
}