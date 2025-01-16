#include <halcyon/types/rectangle.hpp>

namespace hal
{
    // Points.
    template struct point<std::int8_t>;
    template struct point<std::int16_t>;
    template struct point<std::int32_t>;
    template struct point<std::int64_t>;

    template struct point<std::uint8_t>;
    template struct point<std::uint16_t>;
    template struct point<std::uint32_t>;
    template struct point<std::uint64_t>;

    template struct point<float>;
    template struct point<double>;

    // Rectangles.
    template struct rectangle<std::int8_t>;
    template struct rectangle<std::int16_t>;
    template struct rectangle<std::int32_t>;
    template struct rectangle<std::int64_t>;

    template struct rectangle<std::uint8_t>;
    template struct rectangle<std::uint16_t>;
    template struct rectangle<std::uint32_t>;
    template struct rectangle<std::uint64_t>;

    template struct rectangle<float>;
    template struct rectangle<double>;
}