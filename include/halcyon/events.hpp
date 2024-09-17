#pragma once

#include <halcyon/events/holder.hpp>

#include <halcyon/internal/system.hpp>

#include <halcyon/main.hpp>

// events.hpp:
// Master include file for Halcyon Events, plus the events subsystem.

namespace hal
{
    namespace proxy
    {
        class events;
        class video;

        class mouse
        {
        public:
            mouse(pass_key<events>);

            // Get a snapshot of the current mouse state.
            hal::mouse::state state() const;

            // Get the current mouse state relative to the desktop.
            pixel::point pos_abs() const;

            // Get the current mouse state relative to the focus window.
            pixel::point pos_rel() const;
        };

        class keyboard
        {
        public:
            keyboard(pass_key<events>);

            // Get a reference to the keyboard state.
            hal::keyboard::state_reference state_ref() const;
            hal::keyboard::mod_state       mod() const;
        };
    }

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
            void push(const event::holder& eh);

            // Remove all events of a given type from the queue.
            void flush(event::type t);

            // Check whether there are any events waiting to be processed.
            bool pending() const;

            // Check if there are any events of a given type in the queue.
            bool has(event::type t) const;

            // Start/stop text inputs.
            void text_input_start();
            void text_input_stop();

            HAL_NO_SIZE proxy::mouse mouse;
            HAL_NO_SIZE proxy::keyboard keyboard;

        protected:
            // [private] Delegating constructor.
            events();
        };
    }
}
