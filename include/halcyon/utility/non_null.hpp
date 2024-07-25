#pragma once

#include <type_traits>

// utility/non_null.hpp:
// A pointer that cannot be constructed from nullptr.

namespace hal
{
    // non_null can accept const pointers.
    template <typename T>
    concept non_null_compatible = !(std::is_pointer_v<T> || std::is_reference_v<T> || std::is_volatile_v<T>);

    // An ever-so-slightly safer pointer class that cannot be constructed from nullptr.
    // This is purely a compile time thing, and does not check whether the given T* is valid.
    template <non_null_compatible T>
    class non_null
    {
    public:
        non_null(T* ptr)
            : m_ptr { ptr }
        {
        }

        non_null(std::nullptr_t) = delete;

        template <non_null_compatible OtherT>
            requires std::is_same_v<std::remove_const_t<T>, void>
        non_null(non_null<OtherT> other)
            : non_null { static_cast<T*>(other.get()) }
        {
        }
        T* get() const { return m_ptr; }

        T* operator->() const { return m_ptr; }

    private:
        T* m_ptr;
    };
}
