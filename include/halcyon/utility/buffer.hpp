#pragma once

#include <halcyon/utility/metaprogramming.hpp>

#include <memory>
#include <span>

namespace hal
{
    // A class that holds a heap-allocated buffer whose size won't change after construction.
    // So basically a vector, but without the need for storing a separate capacity.
    template <typename T>
    class buffer
    {
    public:
        constexpr buffer()
            : m_size { 0 }
        {
        }

        constexpr buffer(std::uintptr_t sz)
            : m_arr { std::make_unique<T[]>(sz) }
            , m_size { sz }
        {
        }

        // Copy data from a span.
        constexpr buffer(std::span<const T> span)
            : m_arr { std::make_unique<T[]>(span.size()) }
            , m_size { span.size() }
        {
            std::copy(span.begin(), span.end(), begin());
        }

        constexpr buffer(std::initializer_list<T> il)
            : buffer { std::span<const T> { il.begin(), il.end() } }
        {
        }

        constexpr std::size_t size() const
        {
            return m_size;
        }

        constexpr T& operator[](std::size_t idx)
        {
            return m_arr[idx];
        }

        constexpr const T& operator[](std::size_t idx) const
        {
            return m_arr[idx];
        }

        constexpr T* begin()
        {
            return m_arr.get();
        }

        constexpr const T* begin() const
        {
            return m_arr.get();
        }

        constexpr T* end()
        {
            return begin() + size();
        }

        constexpr const T* end() const
        {
            return begin() + size();
        }

        constexpr T* data()
        {
            return m_arr.get();
        }

        constexpr const T* data() const
        {
            return m_arr.get();
        }

    private:
        std::unique_ptr<T[]> m_arr;
        std::uintptr_t       m_size;
    };
}
