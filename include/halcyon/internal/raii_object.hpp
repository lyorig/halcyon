#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include <halcyon/debug.hpp>

// internal/raii_object.hpp:
// An SDL resource base class.

namespace hal
{
    namespace detail
    {
        
        template <typename Underlying_Type>
        class view_base
        {
        public:
            using value_type = Underlying_Type;

            using pointer       = value_type*;
            using const_pointer = const value_type*;

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
            view_base()
                : m_ptr { nullptr }
            {
            }

            view_base(std::nullptr_t) = delete;

            view_base(Underlying_Type* ptr)
                : m_ptr { ptr }
            {
                HAL_ASSERT(valid(), debug::last_error());
            }

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

            Underlying_Type* m_ptr;
        };
    }

    // A view is a non-owning SDL object.
    // It contains non-modifying/querying (const-qualified) member functions,
    // and is then extended by raii_object, which adds modifiers and
    // a destructor that disposes of the contained pointer.
    template <typename Halcyon>
    class view;

    namespace detail
    {
        // An owning SDL object. Extends a view with modifiying functions.
        template <typename Halcyon, typename Underlying, auto Deleter>
            requires std::is_invocable_v<decltype(Deleter), Underlying*>
        class raii_object : public view<Halcyon>
        {
        private:
            using super = view<Halcyon>;

        public:
            using super::super;

            raii_object(raii_object&&)            = default;
            raii_object& operator=(raii_object&&) = default;

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