#pragma once

#include <type_traits>

namespace hal
{
    namespace compile_settings
    {
        constexpr bool main_redefined {
#ifdef main
            true
#else
            false
#endif
        };
    }

    // Check whether the `main` function is correctly written out.
    // On Windows, this will throw a weird compilation error upon failure - haven't found
    // a workaround yet, but in a certain way, this is desirable, since you'll notice it.
    // This is due to SDL's cross-platform hackery; on Windows, it redefines `main`
    // to be `SDL_main`, which hides away the OS' custom GUI application main function.
    // The problem is that this function has an explicit `argc` and `argv` parameter, so
    // while on Unix, you can have `main` function with no parameters, this will not
    // build on Windows. Also note that since `SDL_main` is a normal function that raturns int,
    // you must explicitly return an exit code - it doesn't have an implicit `return 0`.
    template <auto MainFunc>
    constexpr bool is_correct_main { std::is_same_v<decltype(MainFunc), int (*)(int, char**)> };
}
