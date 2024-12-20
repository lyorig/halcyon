#include <halcyon/events.hpp>

#include <ostream>

using namespace hal;

event::push_outcome::push_outcome(int res)
    : m_res { res }
{
}

bool event::push_outcome::valid() const
{
    return m_res >= 0;
}

bool event::push_outcome::filtered() const
{
    return m_res == 0;
}

bool event::push_outcome::pushed() const
{
    return m_res == 1;
}

event::push_outcome::operator bool() const
{
    return valid();
}

std::ostream& event::operator<<(std::ostream& str, push_outcome o)
{
    switch (o.m_res)
    {
    case 0:
        str << "filtered";
        break;

    case 1:
        str << "accepted";
        break;

    default:
        str << "failed";
        break;
    }

    return str;
}

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
        enabled(static_cast<event::type>(type), false);
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

event::push_outcome proxy::events::push(const event::variant& eh)
{
    eh.get(pass_key<events> {}).common.timestamp = ::SDL_GetTicks();

    return ::SDL_PushEvent(&eh.get(pass_key<events> {}));
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

void proxy::events::enabled(event::type t, bool e)
{
    static_assert(false == SDL_DISABLE && true == SDL_ENABLE);

    static_cast<void>(::SDL_EventState(static_cast<Uint32>(t), e));
}

bool proxy::events::enabled(event::type t) const
{
    return static_cast<bool>(::SDL_GetEventState(static_cast<Uint32>(t)));
}

void proxy::events::filter_add(func_ptr<int, void*, event::variant*> filter, void* data)
{
    ::SDL_SetEventFilter(std::bit_cast<SDL_EventFilter>(filter), data);
}

void proxy::events::filter_run(func_ptr<int, void*, event::variant*> filter, void* data)
{
    ::SDL_FilterEvents(std::bit_cast<SDL_EventFilter>(filter), data);
}

void proxy::events::text_input_start()
{
    ::SDL_StartTextInput();
}

void proxy::events::text_input_stop()
{
    ::SDL_StopTextInput();
}
