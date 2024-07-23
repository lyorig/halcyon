#define _CRT_SECURE_NO_WARNINGS

#include <halcyon/internal/rwops.hpp>

#include <halcyon/utility/strutil.hpp>

using namespace hal;

namespace
{
    std::unique_ptr<char[]> wide_to_multibyte(const wchar_t* path)
    {
        const std::size_t len { hal::strlen(path) * sizeof(wchar_t) };

        auto str { std::make_unique<char[]>(len + 1) };

        HAL_ASSERT_VITAL(std::wcstombs(str.get(), path, len) != static_cast<std::size_t>(-1), "RWops: std::wcstombs() returned error value");

        return str;
    }
}

accessor::accessor(const char* path)
    : rwops { ::SDL_RWFromFile(path, "r") }

{
}

accessor::accessor(const wchar_t* path)
    : accessor { wide_to_multibyte(path).get() }
{
}

accessor::accessor(std::string_view path)
    : accessor { path.data() }
{
}

accessor::accessor(std::wstring_view path)
    : accessor { path.data() }
{
}

accessor::accessor(const std::string& path)
    : accessor { path.data() }
{
}

accessor::accessor(const std::wstring& path)
    : accessor { path.data() }
{
}

accessor::accessor(const std::filesystem::path& path)
    : accessor { path.c_str() }
{
}

SDL_RWops* accessor::get(pass_key<image::context>) const
{
    return raii_object::get();
}

SDL_RWops* accessor::use(pass_key<surface>)
{
    return raii_object::release();
}

SDL_RWops* accessor::use(pass_key<font>)
{
    return raii_object::release();
}

SDL_RWops* accessor::use(pass_key<image::context>)
{
    return raii_object::release();
}

outputter::outputter(const char* path)
    : rwops { ::SDL_RWFromFile(path, "w") }

{
}

outputter::outputter(const wchar_t* path)
    : outputter { wide_to_multibyte(path).get() }
{
}

outputter::outputter(std::string_view path)
    : outputter { path.data() }
{
}

outputter::outputter(std::wstring_view path)
    : outputter { path.data() }
{
}

outputter::outputter(const std::string& path)
    : outputter { path.data() }
{
}

outputter::outputter(const std::wstring& path)
    : outputter { path.data() }
{
}

outputter::outputter(const std::filesystem::path& path)
    : outputter { path.c_str() }
{
}

SDL_RWops* outputter::use(pass_key<surface>)
{
    return raii_object::release();
}

SDL_RWops* outputter::use(pass_key<image::context>)
{
    return raii_object::release();
}