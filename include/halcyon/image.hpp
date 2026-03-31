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

        constexpr std::string_view to_string(image::load_format fmt)
        {
            using enum image::load_format;
            using namespace std::string_view_literals;

            switch (fmt)
            {
            case avif:
                return "AVIF"sv;

            case ico:
                return "ICO"sv;

            case cur:
                return "CUR"sv;

            case bmp:
                return "BMP"sv;

            case gif:
                return "GIF"sv;

            case jpg:
                return "JPG"sv;

            case jxl:
                return "JXL"sv;

            case lbm:
                return "LBM"sv;

            case pcx:
                return "PCX"sv;

            case png:
                return "PNG"sv;

            case pnm:
                return "PNM"sv;

            case svg:
                return "SVG"sv;

            case qoi:
                return "QOI"sv;

            case tif:
                return "TIF"sv;

            case xcf:
                return "XCF"sv;

            case xpm:
                return "XPM"sv;

            case xv:
                return "XV"sv;

            case webp:
                return "WebP"sv;

            case unknown:
                return "Unknown"sv;

            default:
                return "[unknown]"sv;
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
