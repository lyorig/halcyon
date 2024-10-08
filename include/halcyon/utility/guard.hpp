#pragma once

#include <halcyon/video/renderer.hpp>

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
            lock(T& obj)
                : m_ref { obj }
            {
                m_ref->lock();
            }

            lock(lref<streaming_texture> tx, streaming_texture::data& out_data)
                requires std::is_same_v<T, streaming_texture>
                : m_ref { tx }
            {
                out_data = m_ref->lock();
            }

            lock(lref<streaming_texture> tx, pixel::rect area, streaming_texture::data& out_data)
                requires std::is_same_v<T, streaming_texture>
                : m_ref { tx }
            {
                out_data = m_ref->lock(area);
            }

            ~lock()
            {
                m_ref->unlock();
            }

        private:
            lref<T> m_ref;
        };

        template <typename T>
        class blend
        {
        public:
            blend(T& obj, blend_mode bm)
                : m_ref { obj }
                , m_old { m_ref->blend() }
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
        class color_mod
        {
        public:
            color_mod(T& obj, color c)
                : m_ref { obj }
                , m_old { m_ref->color_mod() }
            {
                set(c);
            }

            ~color_mod()
            {
                m_ref->color_mod(m_old);
            }

            void set(color c)
            {
                m_ref->color_mod(c);
            }

        private:
            lref<T> m_ref;
            color   m_old;
        };

        class color
        {
        public:
            color(lref<renderer> obj, hal::color c);

            ~color();

            void set(hal::color c);

        private:
            lref<renderer> m_ref;
            hal::color     m_old;
        };

        class target
        {
        public:
            target(lref<renderer> obj, ref<target_texture> tx);

            ~target();

            void set(ref<target_texture> tx);

        private:
            lref<renderer> m_ref;
        };
    }
}
