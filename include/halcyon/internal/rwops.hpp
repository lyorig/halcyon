#pragma once

#include <halcyon/internal/resource.hpp>

#include <halcyon/utility/buffer.hpp>
#include <halcyon/utility/metaprogramming.hpp>
#include <halcyon/utility/pass_key.hpp>

#include "SDL_rwops.h"

#include <filesystem>
#include <span>

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
        enum class mode : bool
        {
            read,
            write
        };

        // This just returns the string if paths are narrow.
        const char* path_cvt(const char* path);

        // Helper class to convert wide strings to narrow ones.
        class wchar_cvt
        {
        public:
            wchar_cvt(const wchar_t* path);

            operator const char*() const;

        private:
            buffer<char> m_buf;
        };

        // This performs the conversion if paths are wide (i.e. on Windows).
        wchar_cvt path_cvt(const wchar_t* path);

        // Base class for SDL_RWops operations.
        template <mode M>
        class rwops : public resource<SDL_RWops, ::SDL_RWclose>
        {
        public:
            rwops(const char* path)
                : resource { ::SDL_RWFromFile(path, M == mode::read ? "r" : "w") }
            {
            }

            rwops(std::nullptr_t) = delete;

            rwops(c_string path)
                : rwops { path.data() }
            {
            }

            rwops(std::string_view path)
                : rwops { path.data() }
            {
            }

            rwops(const std::string& path)
                : rwops { path.data() }
            {
            }

            rwops(const std::filesystem::path& path)
                : rwops { static_cast<const char*>(path_cvt(path.c_str())) }
            {
            }

        protected:
            using resource::resource;
        };
    }

    // An abstraction of various methods of accessing data.
    class accessor : public detail::rwops<detail::mode::read>
    {
    public:
        using rwops::rwops;

        template <std::size_t N>
        accessor(std::span<const std::byte, N> data)
            : rwops { ::SDL_RWFromConstMem(data.data(), N) }
        {
        }

        // get() functions seek the RWops back where they started.
        SDL_RWops* get(pass_key<image::context>) const; // Image format querying.

        // use() functions call release(), so the class gets "consumed".
        SDL_RWops* use(pass_key<surface>);        // BMP loading.
        SDL_RWops* use(pass_key<font>);           // Font loading.
        SDL_RWops* use(pass_key<image::context>); // Image loading.
    };

    // An abstraction of various methods of outputting data.
    class outputter : public detail::rwops<detail::mode::write>
    {
    public:
        using rwops::rwops;

        template <std::size_t N>
        outputter(std::span<std::byte, N> data)
            : rwops { ::SDL_RWFromMem(data.data(), N) }
        {
        }

        // use() functions call release(), so the class gets "consumed".
        SDL_RWops* use(pass_key<surface>);        // BMP saving.
        SDL_RWops* use(pass_key<image::context>); // Image saving.
    };

    namespace meta
    {
        template <typename T>
        concept buffer = requires(const T& x) { std::begin(x); std::end(x); std::size(x); std::data(x); };
    }

    // Shorthand for creating a writeable byte span from a compatible array-like object.
    template <meta::buffer T>
    auto as_bytes(T& buffer)
    {
        return std::as_writable_bytes(std::span(buffer));
    }
    //
    // Shorthand for creating a readable byte span from a compatible array-like object.
    template <meta::buffer T>
    auto as_bytes(const T& buffer)
    {
        return std::as_bytes(std::span(buffer));
    }
}
