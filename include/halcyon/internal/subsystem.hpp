#pragma once

#include <cstdint>

#include <SDL.h>

#include <halcyon/debug.hpp>

#include <halcyon/types/numeric.hpp>

#include <halcyon/utility/pass_key.hpp>

namespace hal
{
    class context;

    namespace detail
    {
        // Available systems. More will be added as Halcyon adds
        // more features from SDL.
        enum class system : u16
        {
            audio  = SDL_INIT_AUDIO,
            video  = SDL_INIT_VIDEO,
            events = SDL_INIT_EVENTS
        };
    }

    // In the detail namespace; not meant to be seen by the end user.
    constexpr std::string_view to_string(detail::system s)
    {
        using enum detail::system;

        switch (s)
        {
        case audio:
            return "Audio";

        case video:
            return "Video";

        case events:
            return "Events";

        default:
            return "[unknown]";
        }
    }

    namespace detail
    {
        template <system S>
        class subinit;

        // A subsystem that doesn't actually (de)initialize anything.
        // This is more of a base class to ensure dependent classes
        // accept proxies as well as actual self-initialized systems.
        template <system S>
        class subsystem
        {
            // Specialized classes provide their own constructors.
            subsystem() = delete;
        };

        // A subsystem that initializes itself.
        template <system S>
        class subinit : public subsystem<S>
        {
        public:
            explicit subinit(context&)
                : subsystem<S> { pass_key<subinit<S>> {} }
            {
                HAL_WARN_IF(initialized(), to_string(S), " subsystem is already initialized");
                HAL_ASSERT_VITAL(::SDL_InitSubSystem(static_cast<Uint32>(S)) == 0, debug::last_error());
                HAL_PRINT(debug::severity::init, to_string(S), " subsystem initialized");
            }

            ~subinit()
            {
                ::SDL_QuitSubSystem(static_cast<Uint32>(S));
                HAL_PRINT(to_string(S), " subsystem destroyed");
            }

            static bool initialized()
            {
                return ::SDL_WasInit(static_cast<Uint32>(S)) == static_cast<Uint32>(S);
            }
        };
    }

    namespace proxy
    {
        using audio  = detail::subsystem<detail::system::audio>;
        using video  = detail::subsystem<detail::system::video>;
        using events = detail::subsystem<detail::system::events>;
    }

    namespace system
    {
        using audio  = detail::subinit<detail::system::audio>;
        using video  = detail::subinit<detail::system::video>;
        using events = detail::subinit<detail::system::events>;
    }

    // All this does is ensure that const references aren't bound to rvalues.
    template <typename T>
    class sysref
    {
    public:
        sysref(T& obj)
            : m_obj { obj }
        {
        }

        sysref(T&&) = delete;

        T& operator()()
        {
            return m_obj;
        }

        const T& operator()() const
        {
            return m_obj;
        }

        T* operator->()
        {
            return &m_obj;
        }

        const T* operator->() const
        {
            return &m_obj;
        }

    private:
        T& m_obj;
    };

    namespace compile_settings
    {
        // Check whether SDL has redefined "main" to be its own SDL_main. Use hal::meta::is_correct_main
        // to determine whether your main function signature is portable.
        constexpr bool main_redefined {
#ifdef main
            true
#else
            false
#endif
        };
    }
}
