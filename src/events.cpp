#include <halcyon/events.hpp>

#include <ostream>

#include <SDL3/SDL_timer.h>

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

std::ostream& hal::event::operator<<(std::ostream& str, push_outcome o)
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

coord::point proxy::events::mouse_pos_abs() const
{
    coord::point ret;

    static_cast<void>(::SDL_GetGlobalMouseState(&ret.x, &ret.y));

    return ret;
}

coord::point proxy::events::mouse_pos_rel() const
{
    coord::point ret;

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

proxy::events::events()
{
}

bool proxy::events::poll(event::variant& ev)
{
    return static_cast<bool>(::SDL_PollEvent(&ev.get(pass_key<events> {})));
}

result<event::variant> proxy::events::poll()
{
    event::variant v;
    return { poll(v), v };
}

void proxy::events::pump()
{
    ::SDL_PumpEvents();
}

event::push_outcome proxy::events::push(event::variant& eh)
{
    eh.get(pass_key<events> {}).common.timestamp = SDL_GetTicksNS();

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
    return ::SDL_HasEvent(static_cast<Uint32>(t));
}

void proxy::events::enabled(event::type t, bool e)
{
    static_cast<void>(::SDL_SetEventEnabled(static_cast<Uint32>(t), e));
}

bool proxy::events::enabled(event::type t) const
{
    return ::SDL_EventEnabled(static_cast<Uint32>(t));
}

void proxy::events::filter_add(func_ptr<int, void*, event::variant*> filter, void* data)
{
    ::SDL_SetEventFilter(std::bit_cast<SDL_EventFilter>(filter), data);
}

void proxy::events::filter_run(func_ptr<int, void*, event::variant*> filter, void* data)
{
    ::SDL_FilterEvents(std::bit_cast<SDL_EventFilter>(filter), data);
}

void proxy::events::text_input_start(ref<window> wnd)
{
    ::SDL_StartTextInput(wnd.get());
}

void proxy::events::text_input_stop(ref<window> wnd)
{
    ::SDL_StopTextInput(wnd.get());
}
