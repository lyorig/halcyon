#include <halcyon/image.hpp>

using namespace hal::image;

context::context(flag_bitset formats)
{
    HAL_WARN_IF(initialized(), "Image context already exists");

    HAL_ASSERT_VITAL(::IMG_Init(formats.mask()) == formats.mask(), ::IMG_GetError());

    HAL_PRINT(debug::severity::init, "Initialized image context [flags: 0x", std::hex, formats.mask(), ']');
}

context::~context()
{
    HAL_ASSERT(initialized(), "Image context not initialized at destruction");

    ::IMG_Quit();

    HAL_PRINT("Destroyed image context");
}

hal::surface context::load(accessor src) const
{
    return { ::IMG_Load_RW(src.use(pass_key<context> {}), true), pass_key<context> {} };
}

hal::surface context::load(accessor src, load_format fmt) const
{
    using enum load_format;

    constexpr std::pair<load_format, func_ptr<SDL_Surface*, SDL_RWops*>> dispatch[] {
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
        if (pair.first == fmt)
            return { pair.second(src.use(pass_key<context> {})), pass_key<context> {} };

    HAL_PANIC("Trying to load image of unknown type");
}

void context::save(ref<const surface> surf, save_format fmt, outputter dst) const
{
    constexpr u8 jpg_quality { 90 };

    switch (fmt)
    {
        using enum save_format;
    case png:
        HAL_ASSERT_VITAL(::IMG_SavePNG_RW(surf->get(), dst.use(pass_key<context> {}), true) == 0, debug::last_error());
        break;

    case jpg:
        HAL_ASSERT_VITAL(::IMG_SaveJPG_RW(surf->get(), dst.use(pass_key<context> {}), true, jpg_quality) == 0, debug::last_error());
        break;
    }
}

load_format context::query(const accessor& src) const
{
    using enum load_format;

    constexpr std::pair<func_ptr<int, SDL_RWops*>, load_format> dispatch[] {
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
        if (pair.first(src.get(pass_key<context> {})) != 0)
            return pair.second;

    return unknown;
}

context::flag_bitset context::flags() const
{
    return ::IMG_Init(0);
}

bool context::initialized()
{
    return ::IMG_Init(0) > 0;
}
