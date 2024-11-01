#pragma once

#include <new>
#include <type_traits>

// utility/optional.hpp:
// Halcyon's version of std::optional.

namespace hal
{
    template <typename T>
    class optional
    {
    public:
        optional()
            : m_valid { false }
        {
        }

        template <typename... CtorArgs>
        optional(CtorArgs&&... args)
            : m_valid { true }
        {
            new (m_data) T { std::forward<CtorArgs>(args)... };
        }

        ~optional()
        {
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
                if (valid())
                    get_unchecked().~T();
            }
        }

        bool valid() const
        {
            return m_valid;
        }

        void reset()
        {
            if (valid())
            {
                get_unchecked().~T();
                m_valid = false;
            }
        }

    private:
        T& get_unchecked()
        {
            return *reinterpret_cast<T*>(m_data);
        }

        std::byte m_data[sizeof(T)];
        bool      m_valid;
    };
}
