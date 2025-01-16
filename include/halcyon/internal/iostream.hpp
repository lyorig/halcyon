#pragma once

#include <halcyon/internal/resource.hpp>

#include <halcyon/utility/buffer.hpp>
#include <halcyon/utility/metaprogramming.hpp>
#include <halcyon/utility/pass_key.hpp>

#include "SDL3/SDL_iostream.h"

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

        // Convert a wide path to a narrow one (i.e. on Windows).
        wchar_cvt path_cvt(const wchar_t* path);

        // Base class for SDL_IOStream operations.
        template <mode M>
        class iostream : public resource<SDL_IOStream, ::SDL_CloseIO>
        {
        public:
            iostream(const char* path)
                : resource { ::SDL_IOFromFile(path, M == mode::read ? "r" : "w") }
            {
            }

            iostream(std::nullptr_t) = delete;

            iostream(c_string path)
                : iostream { path.data() }
            {
            }

            iostream(std::string_view path)
                : iostream { path.data() }
            {
            }

            iostream(const std::string& path)
                : iostream { path.data() }
            {
            }

            iostream(const std::filesystem::path& path)
                : iostream { static_cast<const char*>(path_cvt(path.c_str())) }
            {
            }

        protected:
            using resource::resource;
        };
    }

    // An abstraction of various methods of accessing data.
    class accessor : public detail::iostream<detail::mode::read>
    {
    public:
        using iostream::iostream;

        template <std::size_t N>
        accessor(std::span<const std::byte, N> data)
            : iostream { ::SDL_IOFromConstMem(data.data(), data.size_bytes()) }
        {
        }
    };

    // An abstraction of various methods of outputting data.
    class outputter : public detail::iostream<detail::mode::write>
    {
    public:
        using iostream::iostream;

        template <std::size_t N>
        outputter(std::span<std::byte, N> data)
            : iostream { ::SDL_IOFromMem(data.data(), data.size_bytes()) }
        {
        }
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
