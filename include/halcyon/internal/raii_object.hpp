#pragma once

#include <halcyon/debug.hpp>

// internal/raii_object.hpp:
// SDL view and resource base classes.
// The hierarchy goes like this:
// view<const T> -> view<T> -> raii_object<T> -> T

namespace hal
{
    namespace detail
    {
        template <typename SDL_Type>
        class view_base
        {
        public:
            using value_type = SDL_Type;

            using pointer = value_type*;

        protected:
            view_base(SDL_Type* ptr)
                : m_ptr { ptr }
            {
                HAL_ASSERT(valid(), debug::last_error());
            }

            view_base(std::nullptr_t)
                : m_ptr { nullptr }
            {
            }

        public:
            view_base(const view_base&)            = default;
            view_base& operator=(const view_base&) = default;

            view_base(view_base&& o)
                : m_ptr { o.m_ptr }
            {
                o.m_ptr = nullptr;
            }

            view_base& operator=(view_base&& o)
            {
                m_ptr   = o.m_ptr;
                o.m_ptr = nullptr;

                return *this;
            }

            // Return the underlying pointer to the object. Intended for internal
            // use, or for when you want to interface with SDL to use functions not
            // yet implemented in Halcyon.
            pointer get() const
            {
                return m_ptr;
            }

            // Check whether the object is valid and useable (a.k.a. non-null).
            bool valid() const
            {
                return get() != nullptr;
            }

        protected:
            SDL_Type* m_ptr;
        };
    }

    // A view is a non-owning SDL object.
    // It contains all member functions, and is extended by raii_object,
    // which adds constructors and automatic deallocation.
    template <typename Halcyon_Type>
    class view;

    namespace detail
    {
        // An owning SDL object. Extends a view with modifiying functions.
        template <typename Halcyon_Type, auto Deleter>
            requires std::is_invocable_v<decltype(Deleter), typename view<Halcyon_Type>::pointer>
        class raii_object : public view<Halcyon_Type>
        {
        private:
            using super = view<Halcyon_Type>;

        protected:
            raii_object()
                : super { nullptr }
            {
            }

            raii_object(super::pointer ptr)
                : super { ptr }
            {
            }

        public:
            raii_object(raii_object&&) = default;

            raii_object& operator=(raii_object&& o)
            {
                reset();

                super::m_ptr = o.m_ptr;
                o.m_ptr      = nullptr;

                return *this;
            }

            ~raii_object()
            {
                reset();
            }

            // Free the object.
            void reset()
            {
                if (super::m_ptr)
                    Deleter(super::m_ptr);

                super::m_ptr = nullptr;
            }

        protected:
            // Release the object.
            super::pointer release()
            {
                auto ret { super::get() };

                super::m_ptr = nullptr;

                return ret;
            }
        };
    }
}
