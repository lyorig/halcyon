#pragma once

// misc.hpp:
// Sort-of-important functions.

namespace hal
{
    // Get the name of the current platform.
    const char* platform();

    // Force-unload all SDL libraries. It is recommended to register this function
    // with std::atexit(). Does not affect imaging and TTF components!
    void cleanup();
}
