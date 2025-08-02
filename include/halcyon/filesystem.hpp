#pragma once

#include <halcyon/internal/iostream.hpp>
#include <halcyon/types/c_string.hpp>

namespace hal::fs
{
    // The base path of the application, in other words,
    // the directory in which it resides. This can help when
    // using accessors/outputters, as using relative paths will
    // resolve them with respect to the cwd.
    //
    // Might be slow to call the first time, but SDL internally caches
    // the result for quick calls afterward. However, if you're a proper
    // speed junkie, use `hal::fs::resource_loader`, which internally uses
    // a `std::string_view`, skipping any traversal and/or size calculations
    // required for constructing file paths.
    c_string base_path();

    // See the SDL docs for `SDL_GetPrefPath()`.
    c_string pref_path(c_string org, c_string app);

    // A utility class which caches the base path for fast resolving
    // of relative paths.
    class resource_loader
    {
    public:
        // Default constructor; gets and stores the application directory.
        resource_loader();

        // Resolve a path relative to the application directory.
        // This is basically just a string concatenation, so if
        // you use an absolute path instead of a relative one, it'll
        // probably result in an invalid or otherwise garbled path.
        std::string resolve(std::string_view path) const;

        // Create a file accessor with a path relative to the application directory.
        accessor access(std::string_view path) const;

        // Create a file outputter with a path relative to the application directory.
        outputter output(std::string_view path) const;

        // Get the cached application directory.
        std::string_view base() const;

    private:
        // The reason we can store
        std::string_view m_base;
    };
}
