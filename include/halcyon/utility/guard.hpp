#pragma once

#include <halcyon/internal/raii_object.hpp>

#include <halcyon/video/texture.hpp>

// utility/guard.hpp:
// RAII-based guards for various uses.
// Not useable with CTAD since references are in play.

namespace hal
{
    class target_texture;

    namespace guard
    {
        template <typename T>
        class lock
        {
        public:
            lock(lref<T> obj)
                : m_ref { obj }
            {
                m_ref->lock();
            }

            ~lock()
            {
                m_ref->unlock();
            }

        private:
            lref<T> m_ref;
        };

        template <>
        class lock<streaming_texture>
        {
        public:
            lock(streaming_texture& tx, streaming_texture::data& out_data)
                : m_ref { tx }
            {
                out_data = m_ref->lock();
            }

            lock(lref<streaming_texture> tx, pixel::rect area, streaming_texture::data& out_data)
                : m_ref { tx }
            {
                out_data = m_ref->lock(area);
            }

            ~lock()
            {
                m_ref->unlock();
            }

        private:
            lref<streaming_texture> m_ref;
        };

        template <typename T>
        class blend
        {
        public:
            blend(lref<T> obj, blend_mode bm)
                : m_ref { obj }
                , m_old { obj->blend() }
            {
                set(bm);
            }

            ~blend()
            {
                set(m_old);
            }

            void set(blend_mode bm)
            {
                m_ref->blend(bm);
            }

        private:
            lref<T>    m_ref;
            blend_mode m_old;
        };

        template <typename T>
        class color
        {
        public:
            color(lref<T> obj, hal::color c)
                : m_ref { obj }
                , m_old { obj->color() }
            {
                set(c);
            }

            ~color()
            {
                set(m_old);
            }

            void set(hal::color c)
            {
                m_ref->color(c);
            }

        private:
            lref<T>    m_ref;
            hal::color m_old;
        };

        template <typename T>
        class target
        {
        public:
            target(lref<T> obj, target_texture& tex)
                : m_ref { obj }
            {
                set(tex);
            }

            ~target()
            {
                m_ref->reset_target();
            }

            void set(target_texture& c)
            {
                m_ref->target(c);
            }

        private:
            lref<T> m_ref;
        };
    }
}
