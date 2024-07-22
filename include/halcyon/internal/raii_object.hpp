#pragma once

#include <halcyon/debug.hpp>
#include <halcyon/utility/pass_key.hpp>

// internal/raii_object.hpp:
// SDL resource and reference base classes.

namespace hal
{
    namespace detail
    {
        // An owning SDL object.
        template <typename SDL_Type, auto Deleter>
            requires std::is_invocable_r_v<void, decltype(Deleter), SDL_Type*>
        class raii_object
        {
        public:
            using pointer       = SDL_Type*;
            using const_pointer = const SDL_Type*;

        protected:
            raii_object() = default;

            raii_object(pointer ptr)
                : m_ptr { ptr }
            {
                HAL_ASSERT(valid(), "raii_object not valid after construction");
            }

            pointer release()
            {
                return m_ptr.release();
            }

        public:
            void reset()
            {
                m_ptr.reset();
            }

            pointer get() const
            {
                return m_ptr.get();
            }

            bool valid() const
            {
                return get() != nullptr;
            }

        private:
            struct deleter
            {
                void operator()(pointer ptr) { Deleter(ptr); }
            };

            std::unique_ptr<SDL_Type, deleter> m_ptr;
        };
    };

    class surface;
    class window;
    class renderer;

    // Since a raii_object is nothing more than a pointer anyway, special
    // references are implemented that copy said pointer. It is preferable
    // to use this, since you might save one level of indirection.
    template <typename T>
    class ref
    {
    public:
        ref(T& obj)
            : m_ptr { obj.get() }
        {
        }

        // Specialized constructors:

        // [private] Access a window's surface via window::surface().
        ref(T::pointer ptr, pass_key<window>)
            requires std::is_same_v<std::remove_const_t<T>, surface>
            : ref { ptr }
        {
        }

        // [private] Access a renderer's window via renderer::window().
        ref(T::pointer ptr, pass_key<renderer>)
            requires std::is_same_v<std::remove_const_t<T>, window>
            : ref { ptr }
        {
        }

        // [private] Access a window's renderer via window::renderer().
        ref(T::pointer ptr, pass_key<window>)
            requires std::is_same_v<std::remove_const_t<T>, renderer>
            : ref { ptr }
        {
        }

        ref(const ref&)            = default;
        ref& operator=(const ref&) = default;

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

    private:
        // [private] For use with specialized constuctors.
        ref(T::pointer ptr)
            : m_ptr { ptr }
        {
            HAL_ASSERT(m_ptr != nullptr, "Reference invalid after construction");
        }

        T::pointer m_ptr;
    };

    // Const lvalue reference to an SDL resource. Cannot be bound to an rvalue.
    template <typename T>
    class clref : public ref<const T>
    {
    public:
        using ref<const T>::ref;

        clref(T&&) = delete;
    };
}
