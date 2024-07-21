#pragma once

#include <span>

#include <SDL_rwops.h>

#include <halcyon/internal/raii_object.hpp>

#include <halcyon/utility/concepts.hpp>
#include <halcyon/utility/pass_key.hpp>

struct SDL_Surface;

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
        class rwops : public raii_object<SDL_RWops, ::SDL_RWclose>
        {
        protected:
            using raii_object::raii_object;
        };
    }

    // An abstraction of various methods of accessing data.
    class accessor : public detail::rwops
    {
    public:
        // Access a file.
        accessor(const char* path);
        accessor(std::nullptr_t) = delete;

        // Access a file.
        accessor(std::string_view path);

        // Access a buffer.
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
        // Output to a file.
        outputter(const char* path);
        outputter(std::nullptr_t) = delete;

        // Output to a file.
        outputter(std::string_view path);

        // Output to an array.
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
