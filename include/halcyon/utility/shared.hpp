#pragma once

#include <halcyon/internal/resource.hpp>

namespace hal
{
    // A shared_ptr-like adapter for hal::resource, from which basically every important
    // Halcyon class inherits. Supports CTAD.
    template <typename T>
    class shared
    {
    public:
        using ref_count_t = std::uint32_t;

        shared()
            : m_ptr { nullptr }
            , m_count { nullptr }
        {
        }

        template <typename... Args>
        shared(T&& obj)
            : m_ptr { std::move(obj).release() }
            , m_count { new ref_count_t { get() != nullptr } }
        {
        }

        shared(const shared& other)
            : m_ptr { other.m_ptr }
            , m_count { other.m_count }
        {
            ++(*m_count);
        }

        shared(shared&& other)
            : m_ptr { other.m_ptr }
            , m_count { other.m_count }
        {
            other.m_ptr   = nullptr;
            other.m_count = nullptr;
        }

        shared& operator=(const shared& other)
        {
            m_ptr   = other.m_ptr;
            m_count = other.m_count;

            ++(*m_count);

            return *this;
        }

        shared& operator=(shared&& other)
        {
            m_ptr   = other.m_ptr;
            m_count = other.m_count;

            other.m_ptr   = nullptr;
            other.m_count = nullptr;

            return *this;
        }

        T::pointer get()
        {
            return m_ptr;
        }

        T::const_pointer get() const
        {
            return m_ptr;
        }

        void reset()
        {
            static_cast<void>(release());
        }

        T::pointer release()
        {
            if (!m_count)
                return nullptr;

            const typename T::pointer ret { get() };

            destroy();

            m_ptr   = nullptr;
            m_count = nullptr;

            return ret;
        }

        ref_count_t use_count()
        {
            if (m_count)
                return *m_count;
            else
                return 0;
        }

        ~shared()
        {
            destroy();
        }

    private:
        T::pointer   m_ptr;
        ref_count_t* m_count;

        void destroy()
        {
            if (!m_count)
                return;

            if (--(*m_count) == 0)
            {
                T::deleter::operator()(get());
                delete m_count;

                m_ptr   = nullptr;
                m_count = nullptr;
            }
        }
    };
}
