#pragma once

#include <halcyon/internal/raii_object.hpp>

#include <halcyon/types/color.hpp>
#include <halcyon/video/types.hpp>

// locks.hpp:
// RAII-based locks for various uses.
// Not useable with CTAD since references are in play.

namespace hal
{
    class target_texture;

    namespace lock
    {
        template <typename T>
        class blend
        {
        public:
            blend(ref<T> obj, blend_mode bm)
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
            ref<T>     m_ref;
            blend_mode m_old;
        };

        template <typename T>
        class color
        {
        public:
            color(ref<T> obj, hal::color c)
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
            ref<T>     m_ref;
            hal::color m_old;
        };

        template <typename T>
        class target
        {
        public:
            target(ref<T> obj, target_texture& tex)
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
            ref<T> m_ref;
        };
    }
}