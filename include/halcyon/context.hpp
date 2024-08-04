#pragma once

#include <string_view>

// context.hpp:
// A RAII class that ensures Halcyon deinitialization.

namespace hal
{
    // Get the name of the current platform.
    std::string_view platform();

    // The base context of Halcyon.
    // This class needs to be created first in order to initialize
    // subsystems. Its destruction means everything gets unloaded.
    class context
    {
    public:
        // Initialize Halcyon with no subsystems.
        context() = default;

        context(const context&) = delete;
        context(context&&)      = delete;

        // Forcefully shuts down everything.
        ~context();
    };
}
