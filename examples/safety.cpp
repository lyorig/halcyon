#include <halcyon/video.hpp>

#include <halcyon/image.hpp>
#include <halcyon/ttf.hpp>

int main(int, char*[])
{
    // Halcyon employs some safeguards to make sure you don't use uninitialized systems.
    // For example, here are a few mistakes you could make in SDL:

    SDL_Window* sdl_wnd { ::SDL_CreateWindow(nullptr, // nullptr supplied as string.
        0, 0, 640, 480,
        69420) }; // Random integer passed as bitmask.

    // Wait, did we even initialize the library?!
    // (in this case, the function checks and does this for us, but such altruism is in no way guaranteed)

    SDL_Renderer* sdl_rnd { ::SDL_CreateRenderer(sdl_wnd, -1, 123456789) }; // Same bitmask issue as before.

    // The file path describes what this line does pretty well.
    // The surface wasn't freed, now you suffer the consequences.
    SDL_Texture* sdl_tex { ::SDL_CreateTextureFromSurface(sdl_rnd, ::IMG_Load("memory/leak.png")) };

    ::SDL_DestroyTexture(sdl_tex); // You could very much forget one of these and leak memory.
    ::SDL_DestroyRenderer(sdl_rnd);
    ::SDL_DestroyWindow(sdl_wnd);

    // Now, here's how all this is improved in Halcyon.

    // SDL enrolls its own main function on Windows, which is a common source of headaches.
    // A compile-time check is provided to ensure your program passes this part of the cross-platform check.
    // Commented out to prevent warnings during compilation.
    // static_assert(hal::is_correct_main<main>);

    // Subsystem initialization.
    hal::cleanup_init<hal::subsystem::video> vid;

    // "vid" only makes this function available if it's an lvalue.
    // Temporaries cannot create objects that rely on them.
    // Thus, subsystem initialization is guaranteed at this point, eliminating possible errors.
    hal::window wnd { vid, "Okno", { 640, 480 } };

    // Likewise for "wnd".
    hal::renderer rnd { wnd };

    // Image loading is only possible with an image context, however, it's only needed for
    // the loading itself, and can be discarded immediately afterwards.
    using img = hal::image::context;
    using enum hal::image::init_format;

    hal::static_texture tex { rnd, img { png }.load("assets/file.png") };

    // If you're in debug mode, and your program works up until this point – congrats!
    // Halcyon Debug checks every call that can fail, and – provided that you haven't explicitly
    // disabled it – exit()s the program upon anything failing, along with a message stating what went wrong,
    // and where. For peak performance, this functionality is completely disabled by default upon defining NDEBUG.

    // Deallocation + deinitialization is taken care of, so you can focus on the important part - coding!
    // You also don't have to work with pointers at all. But if your favorite SDL function isn't implemented,
    // all relevant objects provide a get() method which, just like std::unique_ptr, returns the underlying pointer.

    // Other things that improve the experience:
    // - type ecosystem (point, rectangle, color...)
    // - disabled copying of SDL allocated objects
    // - builder patterns for creating/doing stuff with multiple paramaters (message boxes, rendering & blitting)
    // - metaprogramming library with additional checks for possible gotchas
    // - compile time settings (hal::compile_settings)

    return EXIT_SUCCESS;
}

// All of these "initializer" types are empty.
// Use [[no_unique_address]] (defined as HAL_NO_SIZE).
// For example, to wrap all this initialization into a class:
class halcyon
{
public:
    halcyon(std::initializer_list<hal::image::init_format> formats)
        : m_img { formats }
    {
    }

private:
    HAL_NO_SIZE hal::image::context                m_img;
    HAL_NO_SIZE [[maybe_unused]] hal::ttf::context m_ttf;

    HAL_NO_SIZE hal::cleanup_init<hal::subsystem::video> m_video;
};
