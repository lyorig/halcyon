#include <cstdlib>

#include <halcyon/audio.hpp>
#include <halcyon/video.hpp>

#include <halcyon/image.hpp>
#include <halcyon/ttf.hpp>

#include "data.hpp"

// Halcyon testing.
// A single test-runner executable that contains all tests.
// Tests are added to CTest by specifiying the appropriate command-line argument.

#define FAIL_IF(cond, ...)                                      \
    if (cond)                                                   \
    {                                                           \
        HAL_PRINT(__VA_ARGS__, "; ", hal::debug::last_error()); \
        return EXIT_FAILURE;                                    \
    }

namespace test
{
    // Compile-time tests first.
    // You know that these tests pass when this file compiles.
    consteval void scaler()
    {
        constexpr hal::pixel::point src { 50, 100 };
        constexpr hal::pixel::point dst { 100, 200 };

        static_assert(src.scale_width(100) == dst);
        static_assert(src.scale_height(200) == dst);
        static_assert(src * 2 == dst);
    }

    consteval void metaprogramming()
    {
        using ret_t    = std::string;
        using first_t  = std::tuple<int, double, short>;
        using second_t = std::vector<double>;

        using list = hal::meta::type_list<first_t, second_t>;

        static_assert(std::is_same_v<first_t, list::at<0>> && std::is_same_v<second_t, list::at<1>>);

        using info = hal::meta::func_info<ret_t(first_t, second_t)>;

        static_assert(std::is_same_v<ret_t, info::return_type> && std::is_same_v<first_t, info::args::at<0>> && std::is_same_v<second_t, info::args::at<1>>);
    }

    // Debug assertion testing.
    // This test should fail.
    int assert_fail()
    {
        // Failure should occur here.
        HAL_ASSERT(false, "This is intentional.");

        return EXIT_SUCCESS;
    }

    // Resizing a window and checking whether the event handler was notified.
    int window_resize()
    {
        constexpr hal::pixel::point new_size { 120, 120 };

        hal::cleanup_init<hal::system::video> vid;

        hal::window wnd { vid, "HalTest: Window resize", { 640, 480 }, hal::window::flag::hidden };

        hal::event::variant e;

        while (vid.events.poll(e)) // Clear events.
            ;

        wnd.size(new_size);
        vid.events.poll(e);

        FAIL_IF(e.kind() != hal::event::type::window_event, "Event type mismatch (desired \"window\", actual ", e.kind(), ')')
        FAIL_IF(e.window().kind() != hal::event::window::type::size_changed, "Window event type mismatch (desired \"resized\", actual ", e.window().kind(), ')');
        FAIL_IF(e.window().point() != new_size, "Window size mismatch (desired ", new_size, ", actual ", e.window().point(), ')');

        return EXIT_SUCCESS;
    }

    // Basic Halcyon initialization.
    int basic_init()
    {
        FAIL_IF(hal::initialized(hal::system::video), "Video initialized when it shouldn't be");

        hal::outcome                          o;
        hal::cleanup_init<hal::system::video> vid { o };

        FAIL_IF(!o, "Video initialization failed");

        FAIL_IF(!hal::initialized(hal::system::video), "Video not initialized when it should be");

        hal::window   wnd { vid, "HalTest: Basic init", { 640, 480 }, hal::window::flag::hidden };
        hal::renderer rnd { wnd };

        hal::event::variant e;
        vid.events.poll(e);

        rnd.present();

        return EXIT_SUCCESS;
    }

    // Setting and getting the clipboard.
    int clipboard()
    {
        constexpr char text[] { "We can be heroes - just for one day." };

        hal::cleanup_init<hal::system::video> vid;

        FAIL_IF(!vid.clipboard(text), "Could not set clipboard text");

        FAIL_IF(!vid.clipboard_has_text(), "Clipboard doesn't have text when it should");
        FAIL_IF(vid.clipboard() != text, "Clipboard text mismatch");

        return EXIT_SUCCESS;
    }

    // Checking pixel colors in a 2x1 surface.
    int surface_color()
    {
        hal::image::context ictx { hal::image::init_format::png };

        hal::surface s { ictx.load(hal::as_bytes(png_2x1)) };

        FAIL_IF((s[{ 0, 0 }].color() != hal::palette::red || s[{ 1, 0 }].color() != hal::palette::blue), "Surface color mismatch");

        return EXIT_SUCCESS;
    }

    // Sending a quit event and checking whether it gets caught.
    int events()
    {
        hal::cleanup_init<hal::system::events> evt;

        hal::event::variant eh;

        using enum hal::event::type;

        while (evt.poll(eh))
            ;

        eh.kind(quit_requested);

        FAIL_IF(!evt.push(eh), "Couldn't push event");
        FAIL_IF(!evt.poll(eh), "Couldn't poll event");
        FAIL_IF(eh.kind() != quit_requested, "Event type mismatch (desired \"quit_requested\", actual ", eh.kind(), ')');

        constexpr std::string_view text { "aaaaaaaaaabbbbbbbbbbccccccccccd" };

        static_assert(text.size() <= hal::event::text_input::max_size());

        while (evt.poll(eh))
            ;

        eh.kind(hal::event::type::text_input);
        eh.text_input().text(text);
        evt.push(eh);

        FAIL_IF(eh.kind() != text_input, "Event type mismatch (desired \"text_input\", actual ", eh.kind(), ')');

        return EXIT_SUCCESS;
    }

    // Basic TTF initialization.
    int ttf_init()
    {
        {
            hal::ttf::context tctx;

            FAIL_IF(!hal::ttf::initialized(), "TTF context not initialized when it should be");
        }

        FAIL_IF(hal::ttf::initialized(), "TTF context initialized when it shouldn't be");

        return EXIT_SUCCESS;
    }

    int rvalues()
    {
        hal::outcome o;
        hal::cleanup_init<hal::system::video> { o }.clipboard("Hello from HalTest!");

        return EXIT_SUCCESS;
    }

    int outputter()
    {
        hal::surface s { { 2, 2 } };

        s[{ 0, 0 }].color(0xF25022);
        s[{ 1, 0 }].color(0x7FBA00);
        s[{ 0, 1 }].color(0x00A4EF);
        s[{ 1, 1 }].color(0xFFB900);

        FAIL_IF(!s.save("DontSueMeDaddyGates.bmp"), "Could not save 2x2 surface to file");

        std::byte buf[1024];

        FAIL_IF(!s.save(hal::as_bytes(buf)), "Could not save 2x2 surface to buffer");

        return EXIT_SUCCESS;
    }

    int png_check()
    {
        hal::image::context ictx { hal::image::init_format::png };

        using enum hal::image::load_format;

        const hal::image::load_format ret { ictx.query(hal::as_bytes(png_2x1)) };

        FAIL_IF(ret != hal::image::load_format::png, "PNG data not recognized as PNG data");

        return EXIT_SUCCESS;
    }

    int references()
    {
        hal::cleanup_init<hal::system::video> vid;

        hal::window           wnd { vid, "HalTest Views", { 128, 128 } };
        hal::ref<hal::window> r1 = wnd;

        FAIL_IF(wnd.get() != r1->get(), "Reference not same as object after assignment");

        hal::ref<hal::window> r_cpy = r1;

        FAIL_IF(r1->get() != r_cpy->get(), "Reference not same as another reference after assignment");

        hal::ref<hal::window> r_mv = std::move(r1);

        FAIL_IF(r1->valid(), "Moved-from reference is valid");
        FAIL_IF(r_mv->get() != wnd.get(), "Moved-to refence not same as object");

        return EXIT_SUCCESS;
    }

    int audio_init()
    {
        hal::cleanup_init<hal::system::audio> a;

        hal::audio::spec   gotten;
        hal::audio::device dev = hal::audio::builder::device { a }.spec({ 44100, hal::audio::format::f32, 2, 4096 })(gotten);
        hal::audio::stream str { a, { hal::audio::format::i32, 2, 44100 }, { hal::audio::format::u8, 1, 48000 } };

        // Also enumerate devices while we're at it.
        for (hal::audio::device::id_t i = 0; i < a.outputs_size(); ++i)
            HAL_PRINT("Output #", i, ": ", a.outputs_name(i));

        for (hal::audio::device::id_t i = 0; i < a.inputs_size(); ++i)
            HAL_PRINT("Input #", i, ": ", a.inputs_name(i));

        return EXIT_SUCCESS;
    }

    int utilities()
    {
        hal::buffer<int> b1 { 9, 0, 2, 1, 0 }, b2 = b1;

        FAIL_IF(std::memcmp(b1.data(), b2.data(), b1.size_bytes()) != 0, "Buffer data doesn't match");

        return EXIT_SUCCESS;
    }

    // Passing a zeroed-out buffer to a function expecting valid image data.
    int invalid_buffer()
    {
        constexpr std::uint8_t data[1024] {};

        hal::image::context ictx { hal::image::init_format::png };

        const hal::surface s { ictx.load(hal::as_bytes(data)) };

        return s.valid() ? EXIT_FAILURE : EXIT_SUCCESS;
    }

    // Drawing a null texture.
    int invalid_texture()
    {
        hal::cleanup_init<hal::system::video> vid;

        hal::window   wnd { vid, "HalTest: Invalid texture", { 640, 480 }, hal::window::flag::hidden };
        hal::renderer rnd { wnd };

        hal::static_texture tex;

        return rnd.draw(tex)() ? EXIT_FAILURE : EXIT_SUCCESS;
    }

    // Accessing an invalid event.
    // This test should fail.
    int invalid_event()
    {
        hal::cleanup_init<hal::system::events> sys;

        hal::event::variant eh;

        // Failure should occur here.
        eh.text_input().text("amogus sus").window_id(69);

        return EXIT_SUCCESS;
    }
}

int main(int argc, char* argv[])
{
    static_assert(hal::compile_settings::debug_enabled, "HalTest requires debug mode to be enabled");

    constexpr std::pair<std::string_view, hal::func_ptr<int>> tests[] { { "--assert-fail", test::assert_fail },
        { "--window-resize", test::window_resize },
        { "--basic-init", test::basic_init },
        { "--clipboard", test::clipboard },
        { "--surface-color", test::surface_color },
        { "--events", test::events },
        { "--ttf-init", test::ttf_init },
        { "--rvalues", test::rvalues },
        { "--outputter", test::outputter },
        { "--png-check", test::png_check },
        { "--references", test::references },
        { "--audio-init", test::audio_init },
        { "--invalid-buffer", test::invalid_buffer },
        { "--invalid-texture", test::invalid_texture },
        { "--invalid-event", test::invalid_event } };

    FAIL_IF(argc == 1, "No test name given.");

    const auto iter = std::find_if(std::begin(tests), std::end(tests), [&](const auto& pair)
        { return pair.first == argv[1]; });

    FAIL_IF(iter == std::end(tests), "Invalid option specified: ", argv[1]);

    return iter->second();
}
