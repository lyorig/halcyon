#include <iostream>
#include <thread>

#include <halcyon/filesystem.hpp>
#include <halcyon/main.hpp>
#include <halcyon/video.hpp>

#include <halcyon/ttf.hpp>

// text_render.cpp:
// Renders the first command-line argument as text into a window.

int main(int argc, char* argv[])
{
    constexpr hal::pixel::point padding { 20, 20 };
    constexpr hal::font::pt_t   font_size { 128 };

    if (argc == 1)
    {
        std::cout << "Usage: " << argv[0] << " [text]\n";
        return EXIT_FAILURE;
    }

    hal::cleanup_init<hal::subsystem::video> vid;

    hal::window         wnd { vid, "Text renderer", { 100, 100 } };
    hal::event::variant evt;

    // No need to clear now as we fill the whole render target later.
    hal::renderer rnd { wnd };

    // Deallocate as much as we can before the main loop.
    {
        hal::static_texture tex;

        hal::ttf::context        tctx;
        hal::fs::resource_loader ldr;

        std::cerr << ldr.base() << '\n';

        const hal::font    fnt { tctx.make_font(ldr.access("assets/m5x7.ttf"), font_size) };
        const hal::surface surf { fnt.render_solid(argv[1], hal::colors::black) };

        tex = { rnd, surf };

        HAL_PRINT("Pixel format: ", tex.pixel_format().get(), ", type: ", hal::pixel::storage_of(tex.pixel_format().get()));
        HAL_PRINT("Must lock? ", surf.must_lock());

        wnd.size(tex.size().get() + padding);

        rnd.fill(hal::colors::weezer_blue);
        rnd.draw(tex).to(padding / 2).render();
        rnd.present();
    }

    while (true)
    {
        while (vid.events.poll(evt))
        {
            switch (evt.kind())
            {
                using enum hal::event::type;

            case quit_requested:
                return EXIT_SUCCESS;

            default:
                break;
            }
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(20ms);
    }

    return EXIT_SUCCESS;
}
