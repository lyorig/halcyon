#pragma once

#include <halcyon/debug.hpp>

#include <halcyon/types/exception.hpp>
#include <halcyon/utility/metaprogramming.hpp>
#include <halcyon/utility/pass_key.hpp>

#include <memory>

// internal/resource.hpp:
// SDL resource and reference base classes.

namespace hal
{
    namespace detail
    {
        // An owned SDL resource.
        template <typename SDL_Type, auto Deleter>
            requires std::is_invocable_r_v<void, decltype(Deleter), SDL_Type*>
        class resource
        {
        public:
            using type          = SDL_Type;
            using pointer       = type*;
            using const_pointer = const type*;
            using deleter       = meta::struct_functor<Deleter>;

        protected:
            resource() = default;

            resource(pointer ptr)
                : m_ptr { ptr }
            {
                HAL_WARN_IF(!valid(), "Resource initialized with nullptr");
            }

        public:
            void reset(pointer p = pointer())
            {
                m_ptr.reset(p);
            }

            pointer release()
            {
                return m_ptr.release();
            }

            pointer get() const
            {
                return m_ptr.get();
            }

            bool valid() const
            {
                return get() != nullptr;
            }

            bool operator!() const
            {
                return get() == nullptr;
            }

        private:
            std::unique_ptr<type, deleter> m_ptr;
        };
    };

    class surface;
    class window;
    class renderer;

    // Since a resource is nothing more than a pointer anyway, special
    // references are implemented that copy said pointer. It is preferable
    // to use this, since you might save one level of indirection.
    template <typename T>
    class ref
    {
    public:
        // This is a separate function so as to not complicate
        // constructors. It's also kinda unsafe and not meant to
        // be used by the library consumer, so, uhh, yeah.
        static ref from_ptr(T::pointer ptr)
        {
            return ptr;
        }

        ref(T& obj)
            : m_ptr { obj.get() }
        {
        }

        // Think MSVC had an issue with this once.
        ref(ref<std::remove_const_t<T>> r)
            requires std::is_const_v<T>
            : m_ptr { r.get() }
        {
        }

        template <typename OtherT>
            requires(std::is_base_of_v<T, OtherT> && std::is_const_v<T> >= std::is_const_v<OtherT>)
        ref(ref<OtherT> r)
            : m_ptr { r.get() }
        {
        }

        ref(const ref&)            = default;
        ref& operator=(const ref&) = default;

        ref(ref&& other)
            : m_ptr { other.m_ptr }
        {
            other.m_ptr = nullptr;
        }

        ref& operator=(ref&& other)
        {
            m_ptr       = other.m_ptr;
            other.m_ptr = nullptr;

            return *this;
        }

        T& operator()()
        {
            return reinterpret_cast<T&>(*this);
        }

        const T& operator()() const
        {
            return reinterpret_cast<const T&>(*this);
        }

        T* operator->()
        {
            return reinterpret_cast<T*>(this);
        }

        const T* operator->() const
        {
            return reinterpret_cast<const T*>(this);
        }

        T::pointer get() const
        {
            return m_ptr;
        }

    private:
        // [private] For use with specialized constuctors.
        ref(T::pointer ptr)
            : m_ptr { ptr }
        {
        }

        T::pointer m_ptr;
    };

    // Const lvalue reference to an SDL resource. Cannot be bound to an rvalue.
    template <typename T>
    class lref : public ref<T>
    {
    public:
        using ref<T>::ref;

        lref(ref<std::remove_const_t<T>> r)
            : ref<T> { r }
        {
        }

        lref(T&&) = delete;
    };
}
