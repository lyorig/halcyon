#include <halcyon/events.hpp>

using namespace hal;

proxy::mouse::mouse(pass_key<authority_t>)
{
}

mouse::state proxy::mouse::state() const
{
    return { pass_key<proxy::mouse> {} };
}

pixel::point proxy::mouse::pos_abs() const
{
    pixel::point ret;

    static_cast<void>(::SDL_GetGlobalMouseState(&ret.x, &ret.y));

    return ret;
}

pixel::point proxy::mouse::pos_rel() const
{
    pixel::point ret;

    static_cast<void>(::SDL_GetMouseState(&ret.x, &ret.y));

    return ret;
}

proxy::keyboard::keyboard(pass_key<authority_t>)
{
}

keyboard::state_reference proxy::keyboard::state_ref() const
{
    return pass_key<proxy::keyboard> {};
}

keyboard::mod_state proxy::keyboard::mod() const
{
    return pass_key<proxy::keyboard> {};
}

proxy::events::subsystem(pass_key<authority_t>)
    : subsystem {}
{
}

proxy::events::subsystem(pass_key<parent_t>)
    : subsystem {}
{
}

proxy::events::subsystem()
    : mouse { pass_key<proxy::events> {} }
    , keyboard { pass_key<proxy::events> {} }
{
    // Disable unused events.
    for (SDL_EventType type : {
             SDL_LOCALECHANGED,
             SDL_SYSWMEVENT,
             SDL_KEYMAPCHANGED,
             SDL_TEXTEDITING_EXT,
             SDL_JOYAXISMOTION,
             SDL_JOYBALLMOTION,
             SDL_JOYHATMOTION,
             SDL_JOYBUTTONDOWN,
             SDL_JOYBUTTONUP,
             SDL_JOYDEVICEADDED,
             SDL_JOYDEVICEREMOVED,
             SDL_JOYBATTERYUPDATED,
             SDL_CONTROLLERAXISMOTION,
             SDL_CONTROLLERBUTTONDOWN,
             SDL_CONTROLLERBUTTONUP,
             SDL_CONTROLLERDEVICEADDED,
             SDL_CONTROLLERDEVICEREMOVED,
             SDL_CONTROLLERDEVICEREMAPPED,
             SDL_CONTROLLERTOUCHPADDOWN,
             SDL_CONTROLLERTOUCHPADMOTION,
             SDL_CONTROLLERTOUCHPADUP,
             SDL_CONTROLLERSENSORUPDATE,
             // SDL_CONTROLLERSTEAMHANDLEUPDATED, <- Unsupported on Windows, apparently
             SDL_FINGERDOWN,
             SDL_FINGERUP,
             SDL_FINGERMOTION,
             SDL_DOLLARGESTURE,
             SDL_DOLLARRECORD,
             SDL_MULTIGESTURE,
             SDL_DROPFILE,
             SDL_DROPTEXT,
             SDL_DROPBEGIN,
             SDL_DROPCOMPLETE,
             SDL_AUDIODEVICEADDED,
             SDL_AUDIODEVICEREMOVED,
             SDL_SENSORUPDATE,
             SDL_RENDER_TARGETS_RESET,
             SDL_RENDER_DEVICE_RESET })
    {
        ::SDL_EventState(type, SDL_IGNORE);
    }

    // TODO: Remove this in SDL3, it's gonna be done automatically.
    text_input_stop();
}

bool proxy::events::poll(event::holder& eh)
{
    return static_cast<bool>(::SDL_PollEvent(&eh.get(pass_key<subsystem> {})));
}

void proxy::events::pump()
{
    ::SDL_PumpEvents();
}

void proxy::events::push(const event::holder& eh)
{
    eh.get(pass_key<subsystem> {}).common.timestamp = ::SDL_GetTicks();

#ifdef HAL_DEBUG_ENABLED
    const auto ret =
#endif
        ::SDL_PushEvent(&eh.get(pass_key<subsystem> {}));

    HAL_ASSERT(ret >= 0, debug::last_error());
    HAL_WARN_IF(ret == 0, "Pushed event of type ", eh.kind(), " was filtered");
}

void proxy::events::flush(event::type t)
{
    ::SDL_FlushEvent(static_cast<Uint32>(t));
}

bool proxy::events::pending() const
{
    return static_cast<bool>(::SDL_PollEvent(nullptr));
}

bool proxy::events::has(event::type t) const
{
    return ::SDL_HasEvent(static_cast<Uint32>(t)) == SDL_TRUE;
}

void proxy::events::text_input_start()
{
    ::SDL_StartTextInput();
}

void proxy::events::text_input_stop()
{
    ::SDL_StopTextInput();
}
