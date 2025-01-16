#include <halcyon/image.hpp>

#include <halcyon/types/exception.hpp>

using namespace hal;

surface image::load(accessor src)
{
    return ::IMG_Load_IO(src.release(), true);
}

surface image::load(accessor src, load_format fmt)
{
    using enum load_format;

    struct
    {
        load_format                           format;
        func_ptr<SDL_Surface*, SDL_IOStream*> func;
    } constexpr dispatch[] {
        { jpg, ::IMG_LoadPNG_IO },
        { png, ::IMG_LoadPNG_IO },
        { tif, ::IMG_LoadTIF_IO },
        { webp, ::IMG_LoadWEBP_IO },
        { jxl, ::IMG_LoadJXL_IO },
        { avif, ::IMG_LoadAVIF_IO },
        { ico, ::IMG_LoadICO_IO },
        { cur, ::IMG_LoadCUR_IO },
        { bmp, ::IMG_LoadBMP_IO },
        { gif, ::IMG_LoadGIF_IO },
        { lbm, ::IMG_LoadLBM_IO },
        { pcx, ::IMG_LoadPCX_IO },
        { pnm, ::IMG_LoadPNM_IO },
        { svg, ::IMG_LoadSVG_IO },
        { qoi, ::IMG_LoadQOI_IO },
        { xcf, ::IMG_LoadXCF_IO },
        { xpm, ::IMG_LoadXPM_IO },
        { xv, ::IMG_LoadXV_IO }
    };

    for (const auto& pair : dispatch)
        if (pair.format == fmt)
            return pair.func(src.release());

    HAL_PANIC("Trying to load image of unknown type");
}

outcome image::save::png(ref<const surface> surf, outputter dst)
{
    return ::IMG_SavePNG_IO(surf.get(), dst.get(), false);
}

outcome image::save::jpg(ref<const surface> surf, outputter dst, std::uint8_t quality)
{
    return ::IMG_SaveJPG_IO(surf.get(), dst.get(), false, quality);
}

outcome image::save::avif(ref<const surface> surf, outputter dst, std::uint8_t quality)
{
    return ::IMG_SaveAVIF_IO(surf.get(), dst.get(), false, quality);
}

image::load_format image::query(const accessor& src)
{
    using enum load_format;

    struct
    {
        func_ptr<bool, SDL_IOStream*> func;
        load_format                   format;
    } constexpr dispatch[] {
        { ::IMG_isAVIF, avif },
        { ::IMG_isICO, ico },
        { ::IMG_isCUR, cur },
        { ::IMG_isBMP, bmp },
        { ::IMG_isGIF, gif },
        { ::IMG_isJPG, jpg },
        { ::IMG_isJXL, jxl },
        { ::IMG_isLBM, lbm },
        { ::IMG_isPCX, pcx },
        { ::IMG_isPNG, png },
        { ::IMG_isPNM, pnm },
        { ::IMG_isSVG, svg },
        { ::IMG_isQOI, qoi },
        { ::IMG_isTIF, tif },
        { ::IMG_isXCF, xcf },
        { ::IMG_isXPM, xpm },
        { ::IMG_isXV, xv },
        { ::IMG_isWEBP, webp }
    };

    for (const auto& pair : dispatch)
        if (pair.func(src.get()) != 0)
            return pair.format;

    return unknown;
}
