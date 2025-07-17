#pragma once

#include <halcyon/internal/iostream.hpp>
#include <halcyon/types/c_string.hpp>

namespace hal
{
    // The base path of the application, in other words,
    // the directory in which it resides. This can help when
    // using accessors/outputters, as using relative paths will
    // resolve them with respect to the cwd.
    c_string base_path();

    class resource_loader
    {
    public:
        resource_loader();

        std::string resolve(std::string_view path) const;

        accessor  access(std::string_view path) const;
        outputter output(std::string_view path) const;

        std::string_view base() const;

    private:
        std::string_view m_base;
    };
}
