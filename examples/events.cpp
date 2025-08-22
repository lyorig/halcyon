#include <halcyon/main.hpp>
#include <halcyon/video.hpp>

// events.cpp:
// An example event loop in a game.

constexpr hal::pixel::point window_size_mod { 20, 20 };

int main(int, char*[])
{
    hal::cleanup_init<hal::subsystem::video> vid;

    hal::window wnd { vid, "Halcyon Structure Showcase", { 640, 480 }, hal::window::flag::resizable };

    hal::renderer rnd { wnd };

    hal::event::variant eh;

    // The application's main loop starts here.
    while (true) // As long as the program is running...
    {
        rnd.clear();

        while (vid.events.poll(eh)) // As long as there are events to process...
        {
            switch (eh.kind())
            {
                using enum hal::event::type;

            case terminating:
                HAL_PRINT("Program has been terminated!");
                // intentional fallthrough...

            case quit_requested:
                return EXIT_SUCCESS;

            case key_pressed:
                switch (eh.keyboard().key())
                {
                    using enum hal::keyboard::key;

                case esc: // Quit.
                    eh.kind(quit_requested);
                    vid.events.push(eh);
                    break;

                case W: // Increase window size.
                    wnd.size(wnd.size() + window_size_mod);
                    break;

                case S: // Decrease window size.
                    wnd.size(wnd.size() - window_size_mod);
                    break;

                case C: // Paint the background blue.
                    rnd.color(hal::colors::weezer_blue);
                    break;

                default:
                    HAL_PRINT("Unhandled key press: ", eh.keyboard().key(), eh.keyboard().repeat() ? " [REPEAT]" : "");
                }
                break;

            case key_released:
                switch (eh.keyboard().key())
                {
                    using enum hal::keyboard::key;

                case C:
                    rnd.color(hal::colors::black);
                    break;

                default:
                    break;
                }
                break;

            default:
                break;
            }
        }

        rnd.present();
    }

    return EXIT_FAILURE;
}
