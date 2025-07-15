#pragma once

#include <SDL3/SDL_surface.h>
#include <halcyon/types/c_string.hpp>
#include <halcyon/types/rectangle.hpp>

#include "SDL3/SDL_blendmode.h"
#include "SDL3/SDL_pixels.h"

// video/types.hpp:
// Rendering-related types used throughout Halcyon.

namespace hal
{
    namespace literals
    {
        consteval pixel_t operator""_px(unsigned long long val)
        {
            return static_cast<pixel_t>(val);
        }

        consteval pixel_t operator""_px(long double val)
        {
            return static_cast<pixel_t>(val);
        }

        consteval coord_t operator""_crd(unsigned long long val)
        {
            return static_cast<coord_t>(val);
        }

        consteval coord_t operator""_crd(long double val)
        {
            return static_cast<coord_t>(val);
        }
    }

    enum class blend_mode : std::uint8_t
    {
        none           = SDL_BLENDMODE_NONE,
        alpha          = SDL_BLENDMODE_BLEND,
        additive       = SDL_BLENDMODE_ADD,
        color_modulate = SDL_BLENDMODE_MOD,
        color_multiply = SDL_BLENDMODE_MUL
    };

    constexpr c_string to_string(blend_mode bm)
    {
        switch (bm)
        {
            using enum blend_mode;

        case none:
            return "No blend";

        case alpha:
            return "Alpha";

        case additive:
            return "Additive";

        case color_modulate:
            return "Color modulate";

        case color_multiply:
            return "Color multiply";

        default:
            return "[unknown]";
        }
    }

    namespace pixel
    {
        using point = point<pixel_t>;
        using rect  = rectangle<pixel_t>;

        // Formats in which pixels are stored.
        enum class format : std::uint32_t
        {
            unknown = SDL_PIXELFORMAT_UNKNOWN,

            index1_lsb = SDL_PIXELFORMAT_INDEX1LSB,
            index1_msb = SDL_PIXELFORMAT_INDEX1MSB,

            index2_lsb = SDL_PIXELFORMAT_INDEX2LSB,
            index2_msb = SDL_PIXELFORMAT_INDEX2MSB,

            index4_lsb = SDL_PIXELFORMAT_INDEX4LSB,
            index4_msb = SDL_PIXELFORMAT_INDEX4MSB,

            index8 = SDL_PIXELFORMAT_INDEX8,

            rgb332 = SDL_PIXELFORMAT_RGB332,
            rgb565 = SDL_PIXELFORMAT_RGB565,

            bgr565 = SDL_PIXELFORMAT_BGR565,

            rgb24 = SDL_PIXELFORMAT_RGB24,
            bgr24 = SDL_PIXELFORMAT_BGR24,

            rgba4444 = SDL_PIXELFORMAT_RGBA4444,
            rgba5551 = SDL_PIXELFORMAT_RGBA5551,
            rgba8888 = SDL_PIXELFORMAT_RGBA8888,

            rgbx8888 = SDL_PIXELFORMAT_RGBX8888,

            xrgb4444 = SDL_PIXELFORMAT_XRGB4444,
            xrgb1555 = SDL_PIXELFORMAT_XRGB1555,
            xrgb8888 = SDL_PIXELFORMAT_XRGB8888,

            argb4444 = SDL_PIXELFORMAT_ARGB4444,
            argb1555 = SDL_PIXELFORMAT_ARGB1555,
            argb8888 = SDL_PIXELFORMAT_ARGB8888,

            abgr4444 = SDL_PIXELFORMAT_ABGR4444,
            abgr1555 = SDL_PIXELFORMAT_ABGR1555,
            abgr8888 = SDL_PIXELFORMAT_ABGR8888,

            bgra4444 = SDL_PIXELFORMAT_BGRA4444,
            bgra5551 = SDL_PIXELFORMAT_BGRA5551,
            bgra8888 = SDL_PIXELFORMAT_BGRA8888,

            bgrx8888 = SDL_PIXELFORMAT_BGRX8888,

            xbgr4444 = SDL_PIXELFORMAT_XBGR4444,
            xbgr1555 = SDL_PIXELFORMAT_XBGR1555,
            xbgr8888 = SDL_PIXELFORMAT_XBGR8888,

            argb2101010 = SDL_PIXELFORMAT_ARGB2101010,

            // Endianness-dependent aliases.
            // As such, hal::to_string'ing them will return the actual format it's referencing.
            rgba32 = SDL_PIXELFORMAT_RGBA32,
            argb32 = SDL_PIXELFORMAT_ARGB32,
            bgra32 = SDL_PIXELFORMAT_BGRA32,
            abgr32 = SDL_PIXELFORMAT_ABGR32,
            rgbx32 = SDL_PIXELFORMAT_RGBX32,
            xrgb32 = SDL_PIXELFORMAT_XRGB32,
            bgrx32 = SDL_PIXELFORMAT_BGRX32,
            xbgr32 = SDL_PIXELFORMAT_XBGR32,

            yv12 = SDL_PIXELFORMAT_YV12,
            iyuv = SDL_PIXELFORMAT_IYUV,
            yuy2 = SDL_PIXELFORMAT_YUY2,
            uyvy = SDL_PIXELFORMAT_UYVY,
            yvyu = SDL_PIXELFORMAT_YVYU,
            nv12 = SDL_PIXELFORMAT_NV12,
            nv21 = SDL_PIXELFORMAT_NV21
        };

        // Ways in which pixels are stored.
        enum class storage : std::uint8_t
        {
            unknown = SDL_PIXELTYPE_UNKNOWN,

            index1 = SDL_PIXELTYPE_INDEX1,
            index2 = SDL_PIXELTYPE_INDEX2,
            index4 = SDL_PIXELTYPE_INDEX4,
            index8 = SDL_PIXELTYPE_INDEX8,

            packed8  = SDL_PIXELTYPE_PACKED8,
            packed16 = SDL_PIXELTYPE_PACKED16,
            packed32 = SDL_PIXELTYPE_PACKED32,

            array_u8  = SDL_PIXELTYPE_ARRAYU8,
            array_u16 = SDL_PIXELTYPE_ARRAYU16,
            array_u32 = SDL_PIXELTYPE_ARRAYU32,
            array_f16 = SDL_PIXELTYPE_ARRAYF16,
            array_f32 = SDL_PIXELTYPE_ARRAYF32
        };

        enum class bitmap_order : std::uint8_t
        {
            none  = SDL_BITMAPORDER_NONE,
            _4321 = SDL_BITMAPORDER_4321,
            _1234 = SDL_BITMAPORDER_1234,
        };

        enum class packed_order : std::uint8_t
        {
            none = SDL_PACKEDORDER_NONE,
            xrgb = SDL_PACKEDORDER_XRGB,
            rgbx = SDL_PACKEDORDER_RGBX,
            argb = SDL_PACKEDORDER_ARGB,
            rgba = SDL_PACKEDORDER_RGBA,
            xbgr = SDL_PACKEDORDER_XBGR,
            bgrx = SDL_PACKEDORDER_BGRX,
            abgr = SDL_PACKEDORDER_ABGR,
            bgra = SDL_PACKEDORDER_BGRA,
        };

        enum class array_order : std::uint8_t
        {
            none = SDL_ARRAYORDER_NONE,
            rgb  = SDL_ARRAYORDER_RGB,
            rgba = SDL_ARRAYORDER_RGBA,
            argb = SDL_ARRAYORDER_ARGB,
            bgr  = SDL_ARRAYORDER_BGR,
            bgra = SDL_ARRAYORDER_BGRA,
            abgr = SDL_ARRAYORDER_ABGR
        };

        enum class packed_layout : std::uint8_t
        {
            none     = SDL_PACKEDLAYOUT_NONE,
            _332     = SDL_PACKEDLAYOUT_332,
            _4444    = SDL_PACKEDLAYOUT_4444,
            _1555    = SDL_PACKEDLAYOUT_1555,
            _5551    = SDL_PACKEDLAYOUT_5551,
            _565     = SDL_PACKEDLAYOUT_565,
            _8888    = SDL_PACKEDLAYOUT_8888,
            _2101010 = SDL_PACKEDLAYOUT_2101010,
            _1010102 = SDL_PACKEDLAYOUT_1010102
        };

        constexpr storage storage_of(format fmt)
        {
            return static_cast<storage>(SDL_PIXELTYPE(static_cast<SDL_PixelFormat>(fmt)));
        }

        constexpr bitmap_order bitmap_order_of(format fmt)
        {
            return static_cast<bitmap_order>(SDL_PIXELORDER(static_cast<SDL_PixelFormat>(fmt)));
        }

        constexpr packed_order packed_order_of(format fmt)
        {
            return static_cast<packed_order>(SDL_PIXELORDER(static_cast<SDL_PixelFormat>(fmt)));
        }

        constexpr array_order array_order_of(format fmt)
        {
            return static_cast<array_order>(SDL_PIXELORDER(static_cast<SDL_PixelFormat>(fmt)));
        }

        constexpr packed_layout packed_layout_of(format fmt)
        {
            return static_cast<packed_layout>(SDL_PIXELLAYOUT(static_cast<SDL_PixelFormat>(fmt)));
        }

        constexpr std::size_t bits_per_pixel_of(format fmt)
        {
            return SDL_BITSPERPIXEL(static_cast<SDL_PixelFormat>(fmt));
        }

        constexpr std::size_t bytes_per_pixel_of(format fmt)
        {
            return SDL_BYTESPERPIXEL(static_cast<SDL_PixelFormat>(fmt));
        }

        constexpr bool is_indexed(format fmt)
        {
            return SDL_ISPIXELFORMAT_INDEXED(static_cast<SDL_PixelFormat>(fmt));
        }

        constexpr bool has_alpha(format fmt)
        {
            return SDL_ISPIXELFORMAT_ALPHA(static_cast<SDL_PixelFormat>(fmt));
        }
    }

    constexpr c_string to_string(pixel::format fmt)
    {
        using enum pixel::format;

        switch (fmt)
        {
        case unknown:
            return "Unknown";

        case index1_lsb:
            return "Index 1 LSB";

        case index1_msb:
            return "Index 1 MSB";

        case index2_lsb:
            return "Index 2 LSB";

        case index2_msb:
            return "Index 2 MSB";

        case index4_lsb:
            return "Index 4 LSB";

        case index4_msb:
            return "Index 4 MSB";

        case index8:
            return "Index 8";

        case rgb332:
            return "RGB332";

        case rgb565:
            return "RGB565";

        case bgr565:
            return "BGR565";

        case rgb24:
            return "RGB24";

        case bgr24:
            return "BGR24";

        case rgba4444:
            return "RGBA4444";

        case rgba5551:
            return "RGBA5551";

        case rgba8888:
            return "RGBA8888";

        case rgbx8888:
            return "RGBX8888";

        case xrgb4444:
            return "XRGB4444";

        case xrgb1555:
            return "XRGB1555";

        case xrgb8888:
            return "XRGB8888";

        case argb4444:
            return "ARGB4444";

        case argb1555:
            return "ARGB1555";

        case argb8888:
            return "ARGB8888";

        case abgr4444:
            return "ABGR4444";

        case abgr1555:
            return "ABGR1555";

        case abgr8888:
            return "ABGR8888";

        case bgra4444:
            return "BGRA4444";

        case bgra5551:
            return "BGRA5551";

        case bgra8888:
            return "BGRA8888";

        case bgrx8888:
            return "BGRX8888";

        case xbgr4444:
            return "XBGR4444";

        case xbgr1555:
            return "XBGR1555";

        case xbgr8888:
            return "XBGR8888";

        case argb2101010:
            return "ARGB2101010";

        case yv12:
            return "YV12";

        case iyuv:
            return "IYUV";

        case yuy2:
            return "YUY2";

        case uyvy:
            return "UYVY";

        case yvyu:
            return "YVYU";

        case nv12:
            return "NV12";

        case nv21:
            return "NV21";

        default:
            return "[unknown]";
        }
    }

    constexpr c_string to_string(pixel::storage tp)
    {
        using enum pixel::storage;

        switch (tp)
        {
        case unknown:
            return "Unknown";

        case index1:
            return "Index 1";

        case index2:
            return "Index 2";

        case index4:
            return "Index 4";

        case index8:
            return "Index 8";

        case packed8:
            return "Packed 8";

        case packed16:
            return "Packed 16";

        case packed32:
            return "Packed 32";

        case array_u8:
            return "Array u8";

        case array_u16:
            return "Array u16";

        case array_u32:
            return "Array u32";

        case array_f16:
            return "Array f16";

        case array_f32:
            return "Array f32";

        default:
            return "[unknown]";
        }
    }

    constexpr c_string to_string(pixel::bitmap_order o)
    {
        using enum pixel::bitmap_order;

        switch (o)
        {
        case none:
            return "None";

        case _4321:
            return "4321";

        case _1234:
            return "1234";

        default:
            return "[unknown]";
        }
    }

    constexpr c_string to_string(pixel::packed_order o)
    {
        using enum pixel::packed_order;

        switch (o)
        {
        case none:
            return "None";

        case xrgb:
            return "XRGB";

        case rgbx:
            return "RGBX";

        case argb:
            return "ARGB";

        case rgba:
            return "RGBA";

        case xbgr:
            return "XBGR";

        case bgrx:
            return "BGRX";

        case abgr:
            return "ABGR";

        case bgra:
            return "BGRA";

        default:
            return "[unknown]";
        }
    }

    constexpr c_string to_string(pixel::array_order o)
    {
        using enum pixel::array_order;

        switch (o)
        {
        case none:
            return "None";

        case rgb:
            return "RGB";

        case rgba:
            return "RGBA";

        case argb:
            return "ARGB";

        case bgr:
            return "BGR";

        case bgra:
            return "BGRA";

        case abgr:
            return "ABGR";

        default:
            return "[unknown]";
        }
    }

    constexpr c_string to_string(pixel::packed_layout pl)
    {
        using enum pixel::packed_layout;

        switch (pl)
        {
        case none:
            return "None";

        case _332:
            return "332";

        case _4444:
            return "4444";

        case _1555:
            return "1555";

        case _5551:
            return "5551";

        case _565:
            return "565";

        case _8888:
            return "8888";

        case _2101010:
            return "2101010";

        case _1010102:
            return "1010102";
        }
    }

    enum class scale_mode : std::uint8_t
    {
        nearest = SDL_SCALEMODE_NEAREST,
        linear  = SDL_SCALEMODE_LINEAR
    };

    constexpr c_string to_string(scale_mode sm)
    {
        switch (sm)
        {
        case scale_mode::nearest:
            return "Nearest";
        case scale_mode::linear:
            return "Linear";
        default:
            return "[unknown]";
        }
    }

    namespace pixel
    {
        template <typename T>
            requires requires(const T& x) { hal::to_string(x); }
        std::ostream& operator<<(std::ostream& str, const T& obj)
        {
            return str << hal::to_string(obj);
        }
    }

    namespace coord
    {
        using point = point<coord_t>;
        using rect  = rectangle<coord_t>;
    }
}
