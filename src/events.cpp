#include <halcyon/events.hpp>

using namespace hal;

mouse::state proxy::events::mouse_state() const
{
    return { pass_key<proxy::events> {} };
}

pixel::point proxy::events::mouse_pos_abs() const
{
    pixel::point ret;

    static_cast<void>(::SDL_GetGlobalMouseState(&ret.x, &ret.y));

    return ret;
}

pixel::point proxy::events::mouse_pos_rel() const
{
    pixel::point ret;

    static_cast<void>(::SDL_GetMouseState(&ret.x, &ret.y));

    return ret;
}

keyboard::state_reference proxy::events::keyboard_state() const
{
    return pass_key<proxy::events> {};
}

keyboard::mod_state proxy::events::keyboard_mod() const
{
    return pass_key<proxy::events> {};
}

proxy::events::events(pass_key<proxy::video>)
    : events {}
{
}

proxy::events::events()
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

bool proxy::events::poll(event::variant& eh)
{
    return static_cast<bool>(::SDL_PollEvent(&eh.get(pass_key<events> {})));
}

void proxy::events::pump()
{
    ::SDL_PumpEvents();
}

outcome proxy::events::push(const event::variant& eh)
{
    eh.get(pass_key<events> {}).common.timestamp = ::SDL_GetTicks();

    return ::SDL_PushEvent(&eh.get(pass_key<events> {})) >= 0;
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
