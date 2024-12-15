#pragma once

#include <halcyon/debug.hpp>

#include <ostream>
#include <type_traits>
#include <utility>

namespace hal
{
    // This is a weird class but it has its use.
    // First of all, I don't have to write "== 0" after every
    // SDL call, and in addition, after SDL3 chooses to return
    // bools everywhere as well, I can just replace it with:
    //		using outcome = bool;
    // and that's it.
    class outcome
    {
    public:
        enum
        {
            success = 0,
            failure = 1
        };

        constexpr outcome()
            : m_valid { failure }
        {
        }

        // Directly construct from an SDL call.
        constexpr outcome(int func_ret)
            : m_valid { func_ret == success }
        {
        }

        // Use when an SDL call doesn't signal success with 0.
        constexpr outcome(bool func_ret)
            : m_valid { func_ret }
        {
        }

        constexpr operator bool() const
        {
            return m_valid;
        }

        friend std::ostream& operator<<(std::ostream& str, outcome o);

    private:
        bool m_valid;
    };

    template <typename T>
    class result
    {
    public:
        constexpr result() = default;

        constexpr result(outcome func_ret, const T& val)
            requires std::is_trivially_copyable_v<T>
            : m_valid { func_ret }
            , m_value { val }
        {
        }

        constexpr result(outcome func_ret, T&& val)
            requires(!std::is_trivially_move_constructible_v<T>)
            : m_valid { func_ret }
            , m_value { std::move(val) }
        {
        }

        bool valid() const
        {
            return m_valid;
        }

        bool operator!() const
        {
            return !valid();
        }

        constexpr T& get()
        {
            HAL_ASSERT(valid(), "get() called on invalid value; ", debug::last_error());

            return m_value;
        }

        constexpr const T& get() const
        {
            HAL_ASSERT(valid(), "get() called on invalid value; ", debug::last_error());

            return m_value;
        }

        // Get this result's value, disregarding whether it's valid.
        // Useful for when there's multiple potential points of failure and
        // one implies that this result's value will not be used.
        constexpr T& get_unchecked()
            requires std::is_default_constructible_v<T>
        {
            return m_value;
        }

        // Get this result's value, disregarding whether it's valid.
        // Useful for when there's multiple potential points of failure and
        // one implies that this result's value will not be used. (const overload)
        constexpr const T& get_unchecked() const
            requires std::is_default_constructible_v<T>
        {
            return m_value;
        }

        constexpr T get_or(T val)
        {
            return valid() ? get() : val;
        }

        constexpr T& operator*()
        {
            return get();
        }

        constexpr const T& operator*() const
        {
            return get();
        }

        constexpr T* operator->()
        {
            return &get();
        }

        constexpr const T* operator->() const
        {
            return &get();
        }

        friend std::ostream& operator<<(std::ostream& str, const result<T>& res)
        {
            if (res.valid())
                str << res.get();
            else
                str << "[invalid value]";

            return str;
        }

    private:
        outcome m_valid;
        T       m_value;
    };
}
