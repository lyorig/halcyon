# About Halcyon
Halcyon is a barebones SDL wrapper written in modern C++.  
Performance is the main priority here, but work has also been put into making sure there is some level of safety as well.  
Namespaces are not cluttered, everything is neatly organized—proper C++ design, plain and simple.  

# Installation
Halcyon is included into your project via CMake—link against `Halcyon::Halcyon`.

The following libraries are required:
- SDL3
- SDL3_image
- SDL3_ttf

# Usage
Halcyon wraps SDL with several concepts:
- **Init:** Manages SDL subsystem initialization via RAII.
- **Proxy:** Provides subsystem functionality.

> [!TIP]
> These are empty classes. You are encouraged to use them with `[[no_unique_address]]` (also defined as `HAL_NO_SIZE`).

# Example program
This library is still under heavy developement; some namespaces etc. might not be up-to-date, but the structure should remain the same.
```
#include <halcyon/video.hpp>

int main(int argc, char* argv[]) {
  static_assert(hal::is_correct_main<main>);

  hal::cleanup_init<hal::system::video> vid;

  hal::window   wnd{vid, "Example", {640, 480}};
  hal::renderer rnd{wnd};

  hal::event::variant evt;

  while (true) {
    while (vid.events.poll(evt)) {
      if (evt.kind() == hal::event::type::quit_requested)
        return EXIT_SUCCESS; // Normal termination.
    }

    rnd.present_and_clear();
  }

  return EXIT_SUCCESS;
}
```

# Debugging
Halcyon provides several macro functions for a better debugging experience.  
"Vital" variants preserve the condition in case debugging is disabled. Use when your condition has side effects.
- `HAL_PRINT` - print a variadic amount of arguments (must support std::ostream::operator<<).
- `HAL_ASSERT[_VITAL]` - Assert that a condition is true, panic if not.
- `HAL_WARN_IF[_VITAL]` - Print a warning message if a condition is true.
- `HAL_PANIC` - Print where the panic occurred along with variadic user-supplied information, then exit.

These debugging facilities are configured via macros.  
If NDEBUG is not defined, debugging is implicitly enabled.
- `HAL_DEBUG_ENABLED`: Enables all of the aforementioned macros.
- `HAL_DEBUG_ADVANCED`: Enables time logging, and outputs to an additional file. This adds static variables to your program.

> [!IMPORTANT]
> On Windows, GUI applications don't get visible console output by default. As such, if debugging is not completely disabled, `HAL_DEBUG_ADVANCED` gets automatically defined there, so that you have _some_ kind of visible debug output, no matter what platform you're on. I'll try to make this more bearable soon.

# Wishlist
Some things I'd like to eventually implement. 
- Audio subsystem
- GPU subsystem
