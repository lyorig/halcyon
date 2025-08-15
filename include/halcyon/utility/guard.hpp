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
        class lock
        {
        public:
            lock(lref<streaming_texture> tx);

            lock(lref<streaming_texture> tx, pixel::rect area);

            ~lock();

        private:
            lref<streaming_texture> m_ref;

        public:
            result<lock_data> res;
        };

        template <typename T>
        class blend
        {
        public:
            blend(T& obj, blend_mode bm)
                : m_ref { obj }
                , m_old { m_ref->blend().get() }
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
                , m_old { m_ref->color_mod().get() }
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
