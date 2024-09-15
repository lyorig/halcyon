#include <halcyon/audio.hpp>

int main(int argc, char* argv[])
{
    static_assert(hal::meta::is_correct_main<main>);

    hal::system::audio aud;

    hal::audio::device dev { aud.build_device().changes({ hal::audio::change::any }).spec({ 44100, hal::audio::format::f32, 2, 2048 })() };

    {
        hal::audio::device::id_t id;

        for (id = 0; id != aud.outputs.size(); ++id)
            HAL_PRINT("Output #", hal::to_printable_int(id), ": ", aud.outputs.name(id), ", ", aud.outputs.spec(id));

        for (id = 0; id != aud.inputs.size(); ++id)
            HAL_PRINT("Input #", hal::to_printable_int(id), ": ", aud.inputs.name(id), ", ", aud.inputs.spec(id));
    }

    std::byte buffer[512];

    /* fill buffer with data... */

    dev.queue(buffer);

    hal::cleanup();

    return EXIT_SUCCESS;
}
