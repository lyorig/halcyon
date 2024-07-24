#include "halcyon/utility/metaprogramming.hpp"
#include <cstdlib>
#include <halcyon/video.hpp>

int main(int argc, char* argv[])
{
    static_assert(hal::meta::is_correct_main<main>);

    hal::context       c;
    hal::system::video v { c };

    hal::window   wnd { v, "Example", { 640, 480 } };
    hal::renderer rnd { wnd, hal::renderer::flag::vsync };

    {
        const hal::pixel::format pref { rnd.info().formats().front() };

        hal::static_texture t { rnd, { 200, 200 }, pref };

        hal::surface s { t.size(), pref };

        s.fill(hal::palette::cyan);
        t.update(s);

        s.fill(hal::palette::red);
        t.update(s, { 100, 100 });

        s.fill(hal::palette::yellow);
        t.update(s, { 150, 150, 20, 20 });

        HAL_PRINT("Texture size after update is ", t.size());

        rnd.render(t)();
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
    }

    return EXIT_FAILURE;
}
