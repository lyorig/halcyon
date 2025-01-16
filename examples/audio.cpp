#include <halcyon/audio.hpp>

int main(int argc, char* argv[])
{
    hal::cleanup_init<hal::subsystem::audio> aud;

    hal::audio::device dev {
        hal::audio::builder::device { aud }
            .changes({ hal::audio::change::any })
            .spec({ 44100, hal::audio::format::f32, 2, 2048 })()
    };

    {
        hal::audio::device::id_t id;

        for (id = 0; id != aud.outputs_size(); ++id)
            HAL_PRINT("Output #", hal::to_printable_int(id), ": ", aud.outputs_name(id), ", ", aud.outputs_spec(id));

        for (id = 0; id != aud.inputs_size(); ++id)
            HAL_PRINT("Input #", hal::to_printable_int(id), ": ", aud.inputs_name(id), ", ", aud.inputs_spec(id));
    }

    std::byte buffer[512];

    /* fill buffer with data... */

    dev.queue(buffer);

    hal::cleanup();

    return EXIT_SUCCESS;
}
