#pragma once

#include <halcyon/types/exception.hpp>
#include <halcyon/utility/metaprogramming.hpp>

#include <type_traits>
#include <utility>

namespace hal
{
    class outcome
    {
    public:
        // Directly construct from an SDL call.
        constexpr outcome(int func_ret)
            : outcome { func_ret == 0 }
        {
        }

        // Use when an SDL call doesn't signal success with 0.
        constexpr outcome(bool func_ret)
            : m_valid { func_ret }
        {
        }

        constexpr bool valid() const
        {
            return m_valid;
        }

        constexpr operator bool() const
        {
            return valid();
        }

    private:
        bool m_valid;
    };

    template <typename T>
    class result : outcome
    {
    public:
        constexpr result() = default;

        constexpr result(int func_ret, const T& val)
            requires std::is_trivially_copyable_v<T>
            : outcome { func_ret }
            , m_value { val }
        {
        }

        constexpr result(int func_ret, T&& val)
            requires(!std::is_trivially_move_constructible_v<T>)
            : outcome { func_ret }
            , m_value { std::move(val) }
        {
        }

        constexpr T& get()
        {
            if (!valid())
                throw hal::exception {};

            return m_value;
        }

        constexpr const T& get() const
        {
            if (!valid())
                throw hal::exception {};

            return m_value;
        }

    private:
        T m_value;
    };
}
