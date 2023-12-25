#pragma once

#include <SDL2/SDL_error.h>
#include <halcyon/debug.hpp>
#include <lyo/capsule.hpp>

namespace hal
{
    template <typename Type, lyo::func_ptr<void, Type*> Deleter>
    class sdl_object
    {
    public:
        sdl_object() = default;

        sdl_object(Type* object)
            : m_object { object }
        {
            HAL_DEBUG_ASSERT(m_object.get() != nullptr, ::SDL_GetError());
        }

        // Return the underlying pointer to the object. This is only for
        // when you want to interface with SDL to use functions not yet
        // implemented in Halcyon.
        Type* ptr() const
        {
            return m_object.get();
        }

        // Release (reset) the object.
        void release()
        {
            m_object.reset();
        }

    protected:
        void operator=(Type* object)
        {
            HAL_DEBUG_ASSERT_VITAL((m_object.reset(object)).get() != nullptr, ::SDL_GetError());
        }

    private:
        lyo::capsule<Type, Deleter> m_object;
    };
} // namespace hal