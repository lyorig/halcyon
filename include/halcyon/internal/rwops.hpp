#pragma once

#include <filesystem>
#include <span>

#include <SDL_rwops.h>

#include <halcyon/internal/resource.hpp>

#include <halcyon/utility/metaprogramming.hpp>
#include <halcyon/utility/pass_key.hpp>

namespace hal
{
    class surface;
    class font;

    namespace image
    {
        class context;
    }

    namespace detail
    {
        // Base class for SDL_RWops operations.
        class rwops : public resource<SDL_RWops, ::SDL_RWclose>
        {
        protected:
            using resource::resource;
        };
    }

    // An abstraction of various methods of accessing data.
    class accessor : public detail::rwops
    {
    public:
        // File accessors:

        accessor(const char* path);
        accessor(std::nullptr_t) = delete;

        accessor(std::string_view path);
        accessor(const std::string& path);

        accessor(const std::filesystem::path& path);

        // Memory accessors:

        template <std::size_t Size>
        accessor(std::span<const std::byte, Size> buffer)
            : rwops { ::SDL_RWFromConstMem(buffer.data(), buffer.size_bytes()) }
        {
        }

        // get() functions seek the RWops back where they started.
        SDL_RWops* get(pass_key<image::context>) const; // Image format querying.

        // use() functions call release(), so the class gets "consumed".
        SDL_RWops* use(pass_key<surface>);        // BMP loading.
        SDL_RWops* use(pass_key<font>);           // Font loading.
        SDL_RWops* use(pass_key<image::context>); // Image loading.
    };

    namespace meta
    {
        template <typename T>
        concept buffer = requires(const T& x) { std::begin(x); std::end(x); std::size(x); std::data(x); };
    }

    // Shorthand for creating a readable byte span from a compatible array-like object.
    template <meta::buffer T>
    auto as_bytes(const T& buffer)
    {
        return std::as_bytes(std::span(buffer));
    }

    // An abstraction of various methods of outputting data.
    class outputter : public detail::rwops
    {
    public:
        // File outputters:

        outputter(const char* path);
        outputter(std::nullptr_t) = delete;

        outputter(std::string_view path);
        outputter(const std::string& path);

        outputter(const std::filesystem::path& path);

        // Memory outputters:

        template <std::size_t Size>
        outputter(std::span<std::byte, Size> buffer)
            : rwops { ::SDL_RWFromMem(buffer.data(), buffer.size_bytes()) }
        {
        }

        // use() functions call release(), so the class gets "consumed".
        SDL_RWops* use(pass_key<surface>);        // BMP saving.
        SDL_RWops* use(pass_key<image::context>); // Image saving.
    };

    // Shorthand for creating a writeable byte span from a compatible array-like object.
    template <meta::buffer T>
    auto as_bytes(T& buffer)
    {
        return std::as_writable_bytes(std::span(buffer));
    }
}
