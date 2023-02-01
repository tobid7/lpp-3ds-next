#pragma once

#include <citro2d.h>
#include <citro3d.h>
#include <3ds.h>

namespace C2DH
{
    inline C2D_Image LoadTextureMem8888(unsigned char *pixels, int pw, int ph)
    {
        C2D_Image res;
        res.tex = new C3D_Tex;
        res.subtex =
            new Tex3DS_SubTexture({(u16)pw, (u16)ph, 0.0f, 1.0f,
                                   pw / 1024.0f, 1.0f - (ph / 1024.0f)});

        C3D_TexInit(res.tex, 1024, 1024, GPU_RGBA8);
        C3D_TexSetFilter(res.tex, GPU_LINEAR, GPU_LINEAR);
        res.tex->border = 0xFFFFFFFF;
        C3D_TexSetWrap(res.tex, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);

        for (u32 x = 0; x < (u32)pw && x < 1024; x++)
        {
            for (u32 y = 0; y < (u32)ph && y < 1024; y++)
            {
                const u32 dstPos = ((((y >> 3) * (1024 >> 3) + (x >> 3)) << 6) +
                                    ((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) |
                                     ((y & 2) << 2) | ((x & 4) << 2) | ((y & 4) << 3))) *
                                   4;

                const u32 srcPos = (y * pw + x) * 4;
                ((uint8_t *)res.tex->data)[dstPos + 0] =
                    pixels[srcPos + 3];
                ((uint8_t *)res.tex->data)[dstPos + 1] =
                    pixels[srcPos + 2];
                ((uint8_t *)res.tex->data)[dstPos + 2] =
                    pixels[srcPos + 1];
                ((uint8_t *)res.tex->data)[dstPos + 3] =
                    pixels[srcPos + 0];
            }
        }
        return res;
    }
}