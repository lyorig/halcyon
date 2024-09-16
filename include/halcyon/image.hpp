#pragma once

#include <halcyon/surface.hpp>

#include <halcyon/internal/rwops.hpp>
#include <halcyon/utility/enum_bits.hpp>

#include "SDL_image.h"

// image.hpp:
// SDL_image wrappers for image loading.

namespace hal
{
    namespace image
    {
        enum class init_format : u8
        {
            jpg  = IMG_INIT_JPG,
            png  = IMG_INIT_PNG,
            tif  = IMG_INIT_TIF,
            webp = IMG_INIT_WEBP,
            jxl  = IMG_INIT_JXL,
            avif = IMG_INIT_AVIF
        };

        enum class save_format : u8
        {
            png,
            jpg
        };

        enum class load_format : u8
        {
            avif,
            ico,
            cur,
            bmp,
            gif,
            jpg,
            jxl,
            lbm,
            pcx,
            png,
            pnm,
            svg,
            qoi,
            tif,
            xcf,
            xpm,
            xv,
            webp,

            unknown
        };

        // Loads, and provides, image manipulation functionality.
        class context
        {
        public:
            using flag_bitset = enum_bitmask<init_format, int>;

            // Initialize the image context with chosen types.
            context(flag_bitset formats);

            context(const context&) = delete;
            context(context&&)      = delete;

            ~context();

            // Load an image, automatically deducing the format.
            [[nodiscard]] surface load(accessor src) const;

            // Load an image, knowing the format in advance.
            [[nodiscard]] surface load(accessor src, load_format fmt) const;

            // Save a surface with a specified format.
            // JPEG files are currently saved at a hard-coded 90 quality.
            void save(ref<const surface>, save_format fmt, outputter dst) const;

            // Check an image's format.
            // This modifies the accessor, but ultimately sets it back where it was.
            load_format query(const accessor& src) const;

            flag_bitset flags() const;

            static bool initialized();
        };

        static_assert(std::is_empty_v<context>);

        // Ensure calling debug::last_error() gives accurate information.
        static_assert(::IMG_GetError == ::SDL_GetError);
    }

    constexpr std::string_view to_string(image::init_format fmt)
    {
        using enum image::init_format;

        switch (fmt)
        {
        case jpg:
            return "JPG";

        case png:
            return "PNG";

        case tif:
            return "TIF";

        case webp:
            return "WEBP";

        case jxl:
            return "JXL";

        case avif:
            return "AVIF";

        default:
            return "[unknown]";
        }
    }

    constexpr std::string_view to_string(image::save_format fmt)
    {
        using enum image::save_format;

        switch (fmt)
        {
        case png:
            return "PNG";

        case jpg:
            return "JPG";

        default:
            return "[unknown]";
        }
    }

    constexpr std::string_view to_string(image::load_format fmt)
    {
        using enum image::load_format;

        switch (fmt)
        {
        case avif:
            return "AVIF";

        case ico:
            return "ICO";

        case cur:
            return "CUR";

        case bmp:
            return "BMP";

        case gif:
            return "GIF";

        case jpg:
            return "JPG";

        case jxl:
            return "JXL";

        case lbm:
            return "LBM";

        case pcx:
            return "PCX";

        case png:
            return "PNG";

        case pnm:
            return "PNM";

        case svg:
            return "SVG";

        case qoi:
            return "QOI";

        case tif:
            return "TIF";

        case xcf:
            return "XCF";

        case xpm:
            return "XPM";

        case xv:
            return "XV";

        case webp:
            return "WebP";

        case unknown:
            return "Unknown";

        default:
            return "[unknown]";
        }
    }

    namespace image
    {
        template <typename T>
            requires requires(const T& x) { hal::to_string(x); }
        std::ostream& operator<<(std::ostream& str, const T& obj)
        {
            return str << hal::to_string(obj);
        }
    }
}
