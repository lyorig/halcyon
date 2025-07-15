#pragma once

#include <halcyon/surface.hpp>

#include <halcyon/internal/iostream.hpp>
#include <halcyon/utility/enum_bits.hpp>

// image.hpp:
// SDL_image wrappers for image loading.

namespace hal
{
    namespace image
    {
        enum class load_format : std::uint8_t
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

        // Load an image, automatically deducing the format.
        [[nodiscard]] surface load(accessor src);

        // Load an image, knowing the format in advance.
        [[nodiscard]] surface load(accessor src, load_format fmt);

        namespace save
        {
            bool png(ref<const surface> surf, outputter dst);
            bool jpg(ref<const surface> surf, outputter dst, std::uint8_t quality = 90);
            bool avif(ref<const surface> surf, outputter dst, std::uint8_t quality = 90);
        }

        // Check an image's format.
        // This modifies the accessor, but ultimately sets it back where it was.
        load_format query(const accessor& src);

        constexpr c_string to_string(image::load_format fmt)
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
}
