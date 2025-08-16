#include <cstdlib>
#include <thread>

#include <halcyon/main.hpp>
#include <halcyon/video.hpp>

int main(int argc, char* argv[])
{
    hal::cleanup_init<hal::subsystem::video> v;

    hal::window   wnd { v, "Example", { 640, 480 } };
    hal::renderer rnd {};

    {
        const auto pref { hal::pixel::format::rgba32 };

        hal::static_texture t { rnd, rnd.size().get(), pref };

        hal::surface s { t.size().get(), pref };

        s.fill(hal::colors::cyan);
        t.update(s);

        s.fill(hal::colors::orange);
        t.update(s, t.size().get() / 2);

        s.fill(hal::colors::green);
        t.update(s, { { 0, 0 }, s.size() / 4 });

        rnd.draw(t).render();
        rnd.present();
    }

    hal::event::variant h;

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

    return EXIT_FAILURE;
}
