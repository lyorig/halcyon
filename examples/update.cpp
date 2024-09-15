#include <cstdlib>
#include <thread>

#include <halcyon/video.hpp>

int main(int argc, char* argv[])
{
    static_assert(hal::meta::is_correct_main<main>);

    hal::system::video v;

    hal::window   wnd { v, "Example", { 640, 480 } };
    hal::renderer rnd { wnd, hal::renderer::flag::vsync };

    {
        const hal::pixel::format pref { rnd.info().formats().front() };

        hal::static_texture t { rnd, rnd.size(), pref };

        hal::surface s { t.size(), pref };

        s.fill(hal::palette::cyan);
        t.update(s);

        s.fill(hal::palette::orange);
        t.update(s, t.size() / 2);

        s.fill(hal::palette::green);
        t.update(s, { { 0, 0 }, s.size() / 4 });

        rnd.draw(t)();
        rnd.present();
    }

    hal::event::holder h;

    while (true)
    {
        while (v.events.poll(h))
        {
            switch (h.kind())
            {
                using enum hal::event::type;

            case quit_requested:
                return EXIT_SUCCESS;

            case key_pressed:
                switch (h.keyboard().button())
                {
                    using enum hal::keyboard::button;

                case Q:
                    break;

                case W:
                    break;

                case E:
                    break;

                default:
                    break;
                }
                break;

            default:
                break;
            }
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(20ms);
    }

    hal::cleanup();

    return EXIT_FAILURE;
}
