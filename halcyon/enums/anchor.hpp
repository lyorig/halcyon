#pragma once

#include <halcyon/types/point.hpp>

namespace hal {
struct anchor {
    enum pos : lyo::u8 {
        none,
        center,
        top_left,
        top_right,
        bottom_left,
        bottom_right
    };

    template <lyo::arithmetic T>
    static constexpr point<T> resolve(pos anch, point<T> pos, point<T> size)
    {
        switch (anch) {
        case anchor::none:
        case anchor::top_left:
            return pos;

        case anchor::top_right:
            pos.x -= size.x;
            break;

        case anchor::bottom_left:
            pos.y -= size.y;
            break;

        case anchor::bottom_right:
            pos -= size;
            break;

        case anchor::center:
            pos -= size / 2.0;
            break;
        }

        return pos;
    }
};
}