#include <halcyon/internal/rwops.hpp>

using namespace hal;

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

outputter::outputter(std::string_view path)
    : outputter { path.data() }
{
}

outputter::outputter(const std::string& path)
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