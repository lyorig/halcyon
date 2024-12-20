#include <halcyon/image.hpp>

#include <halcyon/types/exception.hpp>

using namespace hal;

namespace
{
    outcome img_init(image::init_bitmask formats)
    {
        return ::IMG_Init(formats.mask()) == formats.mask();
    }
}

image::context::context(init_bitmask formats)
{
    HAL_WARN_IF(initialized(), "Image context already exists");

    if (!img_init(formats))
        throw exception { "Image context creation" };
}

image::context::context(init_bitmask formats, outcome& res)
{
    HAL_WARN_IF(initialized(), "Image context already exists.");

    res = img_init(formats);
}

image::context::~context()
{
    HAL_ASSERT(initialized(), "Image context not initialized at destruction");

    ::IMG_Quit();
}

surface image::context::load(accessor src) const
{
    return { ::IMG_Load_RW(src.use(pass_key<context> {}), true), pass_key<context> {} };
}

surface image::context::load(accessor src, load_format fmt) const
{
    using enum load_format;

    struct
    {
        load_format                        format;
        func_ptr<SDL_Surface*, SDL_RWops*> func;
    } constexpr dispatch[] {
        { jpg, ::IMG_LoadPNG_RW },
        { png, ::IMG_LoadPNG_RW },
        { tif, ::IMG_LoadTIF_RW },
        { webp, ::IMG_LoadWEBP_RW },
        { jxl, ::IMG_LoadJXL_RW },
        { avif, ::IMG_LoadAVIF_RW },
        { ico, ::IMG_LoadICO_RW },
        { cur, ::IMG_LoadCUR_RW },
        { bmp, ::IMG_LoadBMP_RW },
        { gif, ::IMG_LoadGIF_RW },
        { lbm, ::IMG_LoadLBM_RW },
        { pcx, ::IMG_LoadPCX_RW },
        { pnm, ::IMG_LoadPNM_RW },
        { svg, ::IMG_LoadSVG_RW },
        { qoi, ::IMG_LoadQOI_RW },
        { xcf, ::IMG_LoadXCF_RW },
        { xpm, ::IMG_LoadXPM_RW },
        { xv, ::IMG_LoadXV_RW }
    };

    for (const auto& pair : dispatch)
        if (pair.format == fmt)
            return { pair.func(src.use(pass_key<context> {})), pass_key<context> {} };

    HAL_PANIC("Trying to load image of unknown type");
}

outcome image::context::save(ref<const surface> surf, save_format fmt, outputter dst) const
{
    constexpr std::uint8_t jpg_quality { 90 };

    switch (fmt)
    {
        using enum save_format;
    case png:
        return ::IMG_SavePNG_RW(surf.get(), dst.use(pass_key<context> {}), true);
        break;

    case jpg:
        return ::IMG_SaveJPG_RW(surf.get(), dst.use(pass_key<context> {}), true, jpg_quality);
        break;
    }
}

image::load_format image::context::query(const accessor& src) const
{
    using enum load_format;

    struct
    {
        func_ptr<int, SDL_RWops*> func;
        load_format               format;
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
        if (pair.func(src.get(pass_key<context> {})) != 0)
            return pair.format;

    return unknown;
}

image::init_bitmask image::context::flags() const
{
    return static_cast<init_format>(::IMG_Init(0));
}

bool image::initialized()
{
    return ::IMG_Init(0) > 0;
}
