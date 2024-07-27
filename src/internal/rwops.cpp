#include <halcyon/internal/rwops.hpp>

#include <halcyon/utility/buffer.hpp>

using namespace hal;

namespace
{
    // If filesystem paths are narrow, don't bother.
    const char* path_cvt(const char* path)
    {
        return path;
    }

    class wchar_cvt
    {
    public:
        wchar_cvt(const wchar_t* path)
            : m_buf { std::wcslen(path) * sizeof(wchar_t) + 1 }
        {
            HAL_ASSERT_VITAL(std::wcstombs(m_buf.data(), path, m_buf.size()) != -1, "Couldn't convert wchar_t string to char string");
        }

        operator const char*() const
        {
            return m_buf.data();
        }

    private:
        buffer<char> m_buf;
    };

    // Otherwise, do indeed bother, and do it in the simplest way possible.
    wchar_cvt path_cvt(const wchar_t* path)
    {
        return path;
    }
}

accessor::accessor(const char* path)
    : rwops { ::SDL_RWFromFile(path, "r") }

{
}

accessor::accessor(std::string_view path)
    : accessor { path.data() }
{
}

accessor::accessor(const std::string& path)
    : accessor { path.data() }
{
}

accessor::accessor(const std::filesystem::path& path)
    : accessor { path_cvt(path.c_str()) }
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

outputter::outputter(std::string_view path)
    : outputter { path.data() }
{
}

outputter::outputter(const std::string& path)
    : outputter { path.data() }
{
}

outputter::outputter(const std::filesystem::path& path)
    : outputter { path_cvt(path.c_str()) }
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
