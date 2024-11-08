#pragma once

#include <halcyon/events/holder.hpp>

#include <halcyon/system.hpp>

#include <halcyon/main.hpp>

// events.hpp:
// Master include file for Halcyon Events, plus the events subsystem.

namespace hal
{
    namespace proxy
    {
        // A system that represents the event queue.
        class events
        {
        public:
            constexpr static system system { system::events };

            // [private] The video subsystem implicitly initializes events.
            events(pass_key<video>);

            // Collect pending events.
            // This is usually not necessary - poll() calls it in an event loop.
            void pump();

            // Get and remove an event from the queue.
            // Returns whether there are still events to process.
            bool poll(event::holder& eh);

            // Push an event onto the queue.
            outcome push(const event::holder& eh);

            // Remove all events of a given type from the queue.
            void flush(event::type t);

            // Check whether there are any events waiting to be processed.
            bool pending() const;

            // Check if there are any events of a given type in the queue.
            bool has(event::type t) const;

            keyboard::state_reference keyboard_state() const;
            keyboard::mod_state       keyboard_mod() const;

            // Get a snapshot of the current mouse state.
            mouse::state mouse_state() const;

            // Get the current mouse state relative to the desktop.
            pixel::point mouse_pos_abs() const;

            // Get the current mouse state relative to the focus window.
            pixel::point mouse_pos_rel() const;

            // Start/stop text inputs.
            void text_input_start();
            void text_input_stop();

        protected:
            // [private] Delegating constructor.
            events();
        };
    }
}
