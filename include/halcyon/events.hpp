#pragma once

#include <halcyon/events/variant.hpp>

#include <halcyon/subsystem.hpp>

#include <halcyon/main.hpp>

// events.hpp:
// Master include file for Halcyon Events, plus the events subsystem.

namespace hal
{
    namespace event
    {
        // A special type of outcome that accounts for pushed events being filtered.
        class push_outcome
        {
        public:
            push_outcome(int res);

            // Returns true if the call didn't fail, i.e. the event was accepted or filtered.
            bool valid() const;

            // Returns true if the call was filtered. This does not mean it was ignored!
            bool filtered() const;

            // Returns whether the event wasn't filtered and was pushed into the queue.
            bool pushed() const;

            // Returns valid().
            operator bool() const;

            friend std::ostream& operator<<(std::ostream& str, push_outcome o);

        private:
            int m_res;
        };
    }

    namespace proxy
    {
        // A system that represents the event queue.
        class events
        {
        public:
            // [private] The video subsystem implicitly initializes events.
            events(pass_key<video>);

            // Collect pending events.
            // This is usually not necessary - poll() calls it in an event loop.
            void pump();

            // Get and remove an event from the queue.
            // Returns whether there are still events to process.
            bool poll(event::variant& v);

            // Get and remove an event from the queue.
            // Returns an invalid result if the queue is empty.
            result<event::variant> poll();

            // Push an event onto the queue.
            event::push_outcome push(const event::variant& eh);

            // Remove all events of a given type from the queue.
            void flush(event::type t);

            // Check whether there are any events waiting to be processed.
            bool pending() const;

            // Check if there are any events of a given type in the queue.
            bool has(event::type t) const;

            void enabled(event::type t, bool e);
            bool enabled(event::type t) const;

            // Add a filter that checks all pushed events.
            // If [filter] returns 1, the event is pushed to the event queue.
            // If [filter] returns 0, the event is dropped and not added to the event queue.
            void filter_add(func_ptr<int, void*, event::variant*> filter, void* data);

            // Filter all events in the queue immediately.
            // If [filter] returns 1, the event is kept in the event queue.
            // If [filter] returns 0, the event is dropped from the event queue.
            void filter_run(func_ptr<int, void*, event::variant*> filter, void* data);

            keyboard::state_reference keyboard_state() const;
            keyboard::mod_state       keyboard_mod() const;

            // Get a snapshot of the current mouse state.
            mouse::state mouse_state() const;

            // Get the current mouse state relative to the desktop.
            coord::point mouse_pos_abs() const;

            // Get the current mouse state relative to the focus window.
            coord::point mouse_pos_rel() const;

            // Start/stop text inputs.
            void text_input_start(ref<window> wnd);
            void text_input_stop(ref<window> wnd);
        };
    }
}
