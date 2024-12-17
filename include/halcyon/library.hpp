#pragma once

// library.hpp:
// Platform-dependent library routines.

#include <halcyon/types/c_string.hpp>

namespace hal
{
    class library
    {
    public:
        library();
        library(c_string name);

        ~library();

        bool valid() const;

        template <typename T>
            requires std::is_function_v<T>
        T* function(c_string name)
        {
            return reinterpret_cast<T*>(base_function(name));
        }

    private:
        void* base_function(c_string name) const;

        void* m_handle;
    };
}
