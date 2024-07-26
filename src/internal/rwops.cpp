#include <halcyon/internal/rwops.hpp>

#include <halcyon/utility/strutil.hpp>

using namespace hal;

namespace
{
    template <typename CharT>
    class path_to_cstring
    {
    private:
        static constexpr bool is_narrow { std::is_same_v<CharT, char> };
        using storage = std::conditional_t<is_narrow, const char*, std::string>;

    public:
        path_to_cstring(const std::filesystem::path& p)
            requires is_narrow
            : m_str { p.c_str() }
        {
        }

        path_to_cstring(const std::filesystem::path& p)
            requires(!is_narrow)
            : m_str { p.string() }
        {
        }

        const char* operator()() const
        {
            if constexpr (is_narrow)
            {
                return m_str;
            }

            else
            {
                return m_str.c_str();
            }
        }

    private:
        storage m_str;
    };

    using p2c = path_to_cstring<std::filesystem::path::value_type>;
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
    : accessor { p2c { path }() }
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
    : outputter { p2c { path }() }
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
