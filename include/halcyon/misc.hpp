#pragma once

// misc.hpp:
// Sort-of-important functions.

namespace hal
{
    // Get the name of the current platform.
    const char* platform();

    // Force-unload all SDL libraries. Systems are destroyed automatically via RAII,
    // but it is nonetheless recommended to call this function at the end of your program,
    // for example by registering it with std::atexit(). Does not affect imaging and TTF!
    void cleanup();
}
