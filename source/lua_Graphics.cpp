#include "utils.h"
#include <3ds.h>
#include <C2D_Helper.hpp>
#include <Graphics.hpp>
#include <luaplayer.hpp>
#include <string.h>

#include <iostream>

bool c3d_Frame_en = false;

#define RGBA8(r, g, b, a)                                                      \
  ((((r)&0xFF) << 0) | (((g)&0xFF) << 8) | (((b)&0xFF) << 16) |                \
   (((a)&0xFF) << 24))

#define stringify(str) #str
#define VariableRegister(lua, value)                                           \
  do {                                                                         \
    lua_pushinteger(lua, value);                                               \
    lua_setglobal(lua, stringify(value));                                      \
  } while (0)

int cur_screen;

C3D_RenderTarget *t_targets[3];

static int lua_init(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 0)
    return luaL_error(L, "wrong number of arguments");
#endif
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  t_targets[0] = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
  t_targets[1] = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
  t_targets[2] = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);
  C2D_Prepare();
  cur_screen = 2;
  c3d_Frame_en = true;
  return 0;
}

static int lua_term(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 0)
    return luaL_error(L, "wrong number of arguments");
#endif
  cur_screen = 2;
  C2D_Fini();
  C3D_Fini();
  c3d_Frame_en = false;
  return 0;
}

static int lua_refresh(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if ((argc != 1) && (argc != 2))
    return luaL_error(L, "wrong number of arguments");
#endif
  int screen = luaL_checkinteger(L, 1);
  int side = 0;
  if (argc == 2)
    side = luaL_checkinteger(L, 2);
  gfxScreen_t my_screen;
  gfx3dSide_t eye;
  cur_screen = screen;
  if (screen == 0)
    my_screen = GFX_TOP;
  else
    my_screen = GFX_BOTTOM;
  if (side == 0)
    eye = GFX_LEFT;
  else
    eye = GFX_RIGHT;
  C3D_FrameBegin(0);
  if (eye == GFX_RIGHT && my_screen == GFX_TOP)
    C2D_SceneBegin(t_targets[2]);
  else if (eye == GFX_LEFT && my_screen == GFX_TOP)
    C2D_SceneBegin(t_targets[0]);
  else if (eye == GFX_LEFT && my_screen == GFX_BOTTOM)
    C2D_SceneBegin(t_targets[1]);
  return 0;
}

static int lua_end(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 0)
    return luaL_error(L, "wrong number of arguments");
#endif
  C3D_FrameEnd(0);
  return 0;
}

static int lua_flip(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 0)
    return luaL_error(L, "wrong number of arguments");
#endif
  std::cout << "The function " << stringify(lua_flip)
            << " is completly \ndeprecated and will never\nbe supportet again!"
            << std::endl;
  return 0;
}

static int lua_rect(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 5 && argc != 6)
    return luaL_error(L, "wrong number of arguments");
#endif
  float x1 = luaL_checknumber(L, 1);
  float x2 = luaL_checknumber(L, 2);
  float y1 = luaL_checknumber(L, 3);
  float y2 = luaL_checknumber(L, 4);
  float radius = 0;
  if (x2 < x1) {
    int tmp = x2;
    x2 = x1;
    x1 = tmp;
  }
  if (y2 < y1) {
    int tmp = y2;
    y2 = y1;
    y1 = tmp;
  }
  u32 color = luaL_checkinteger(L, 5);
#ifndef SKIP_ERROR_HANDLING
  if (cur_screen != 1 && cur_screen != 0)
    return luaL_error(L, "you need to call initBlend to use GPU rendering");
#endif
  if (argc == 6)
    radius = luaL_checknumber(L, 6);
  if (radius == 0)
    C2D_DrawRectSolid(x1, y1, 0.5, x2 - x1, y2 - y1,
                      RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF,
                            (color)&0xFF, (color >> 24) & 0xFF));
  else
    std::cout << "Drawing Rotated Rectangles\nis not Supported by C2D!";
  return 0;
}

static int lua_fillcircle(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 4)
    return luaL_error(L, "wrong number of arguments");
#endif
  float x = luaL_checknumber(L, 1);
  float y = luaL_checknumber(L, 2);
  int radius = luaL_checkinteger(L, 3);
  u32 color = luaL_checkinteger(L, 4);
#ifndef SKIP_ERROR_HANDLING
  if (cur_screen != 1 && cur_screen != 0)
    return luaL_error(L, "you need to call initBlend to use GPU rendering");
#endif
  C2D_DrawCircleSolid(x, y, 0.5, radius,
                      RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF,
                            (color)&0xFF, (color >> 24) & 0xFF));
  return 0;
}

static int lua_line(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 5)
    return luaL_error(L, "wrong number of arguments");
#endif
  float x1 = luaL_checknumber(L, 1);
  float x2 = luaL_checknumber(L, 2);
  float y1 = luaL_checknumber(L, 3);
  float y2 = luaL_checknumber(L, 4);
#ifndef SKIP_ERROR_HANDLING
  if (cur_screen != 1 && cur_screen != 0)
    return luaL_error(L, "you need to call initBlend to use GPU rendering");
#endif
  u32 color = luaL_checkinteger(L, 5);
  C2D_DrawLine(x1, y1,
               RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF,
                     (color >> 24) & 0xFF),
               x2, y2,
               RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF,
                     (color >> 24) & 0xFF),
               1, 1);
  return 0;
}

static int lua_emptyrect(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 5)
    return luaL_error(L, "wrong number of arguments");
#endif
  float x1 = luaL_checknumber(L, 1);
  float x2 = luaL_checknumber(L, 2);
  float y1 = luaL_checknumber(L, 3);
  float y2 = luaL_checknumber(L, 4);
#ifndef SKIP_ERROR_HANDLING
  if (cur_screen != 1 && cur_screen != 0)
    return luaL_error(L, "you need to call initBlend to use GPU rendering");
#endif
  u32 color = luaL_checkinteger(L, 5);
  C2D_DrawLine(x1, y1,
               RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF,
                     (color >> 24) & 0xFF),
               x1, y2,
               RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF,
                     (color >> 24) & 0xFF),
               1, 1);
  C2D_DrawLine(x2, y1,
               RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF,
                     (color >> 24) & 0xFF),
               x2, y2,
               RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF,
                     (color >> 24) & 0xFF),
               1, 1);
  C2D_DrawLine(x1, y2,
               RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF,
                     (color >> 24) & 0xFF),
               x2, y2,
               RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF,
                     (color >> 24) & 0xFF),
               1, 1);
  C2D_DrawLine(x1, y1,
               RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF,
                     (color >> 24) & 0xFF),
               x2, y1,
               RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF,
                     (color >> 24) & 0xFF),
               1, 1);
  return 0;
}

static int lua_loadimg(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1)
    return luaL_error(L, "wrong number of arguments");
#endif
  char *text = (char *)(luaL_checkstring(L, 1));
  fileStream fileHandle;
  u32 bytesRead;
  u16 magic;
  u64 long_magic;
  if (strncmp("romfs:/", text, 7) == 0) {
    fileHandle.isRomfs = true;
    FILE *handle = fopen(text, "r");
#ifndef SKIP_ERROR_HANDLING
    if (handle == NULL)
      return luaL_error(L, "file doesn't exist.");
#endif
    fileHandle.handle = (u32)handle;
  } else {
    fileHandle.isRomfs = false;
    FS_Path filePath = fsMakePath(PATH_ASCII, text);
    Result ret =
        FSUSER_OpenFileDirectly(&fileHandle.handle, ARCHIVE_SDMC, filePath,
                                filePath, FS_OPEN_READ, 0x00000000);
#ifndef SKIP_ERROR_HANDLING
    if (ret)
      return luaL_error(L, "file doesn't exist.");
#endif
  }
  FS_Read(&fileHandle, &bytesRead, 0, &magic, 2);
  Bitmap *bitmap;
  if (magic == 0x5089) {
    FS_Read(&fileHandle, &bytesRead, 0, &long_magic, 8);
    FS_Close(&fileHandle);
    if (long_magic == 0x0A1A0A0D474E5089)
      bitmap = decodePNGfile(text);
  } else if (magic == 0x4D42) {
    FS_Close(&fileHandle);
    bitmap = decodeBMPfile(text);
  } else if (magic == 0xD8FF) {
    FS_Close(&fileHandle);
    bitmap = decodeJPGfile(text);
  }
#ifndef SKIP_ERROR_HANDLING
  if (!bitmap)
    return luaL_error(L, "Error loading image");
#endif
  if (bitmap->bitperpixel == 24) {
    int length = (bitmap->width * bitmap->height) << 2;
    u8 *real_pixels = new u8[length];
    int i = 0;
    int z = 0;
    while (i < length) {
      real_pixels[i] = bitmap->pixels[z];
      real_pixels[i + 1] = bitmap->pixels[z + 1];
      real_pixels[i + 2] = bitmap->pixels[z + 2];
      real_pixels[i + 3] = 0xFF;
      i = i + 4;
      z = z + 3;
    }
    delete[] bitmap->pixels;
    bitmap->pixels = real_pixels;
  }
  C2D_Image ret =
      C2DH::LoadTextureMem8888(bitmap->pixels, bitmap->width, bitmap->height);
  C2D_Image *tex = new C2D_Image;
  tex = &ret;
  gpu_text *result = new gpu_text;
  result->magic = 0x4C545854;
  result->tex = tex;
  result->width = bitmap->width;
  result->height = bitmap->height;
  delete[] bitmap->pixels;
  delete[] bitmap;
  lua_pushinteger(L, (u32)(result));
  return 1;
}

static int lua_convert(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1)
    return luaL_error(L, "wrong number of arguments");
#endif
  Bitmap *bitmap = (Bitmap *)(luaL_checkinteger(L, 1));
#ifndef SKIP_ERROR_HANDLING
  if (bitmap->magic != 0x4C494D47)
    return luaL_error(L, "attempt to access wrong memory block type");
#endif
  u8 *real_pixels;
  u8 *flipped =
      new u8[bitmap->width * bitmap->height * (bitmap->bitperpixel >> 3)];
  flipped = flipBitmap(flipped, bitmap);
  int length = (bitmap->width * bitmap->height) << 2;
  if (bitmap->bitperpixel == 24) {
    real_pixels = new u8[length];
    int i = 0;
    int z = 0;
    while (i < length) {
      real_pixels[i] = flipped[z + 2];
      real_pixels[i + 1] = flipped[z + 1];
      real_pixels[i + 2] = flipped[z];
      real_pixels[i + 3] = 0xFF;
      i = i + 4;
      z = z + 3;
    }
    free(flipped);
  } else {
    real_pixels = flipped;
    int i = 0;
    while (i < length) {
      u8 tmp = real_pixels[i + 2];
      real_pixels[i + 2] = real_pixels[i];
      real_pixels[i] = tmp;
      i = i + 4;
    }
  }
  C2D_Image ret =
      C2DH::LoadTextureMem8888(bitmap->pixels, bitmap->width, bitmap->height);
  C2D_Image *tex = new C2D_Image;
  tex = &ret;
  gpu_text *result = new gpu_text;
  result->magic = 0x4C545854;
  result->tex = tex;
  result->width = bitmap->width;
  result->height = bitmap->height;
  delete[] real_pixels;
  lua_pushinteger(L, (u32)(result));
  return 1;
}

static int lua_drawimg(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 3 && argc != 4)
    return luaL_error(L, "wrong number of arguments");
#endif
  float x = luaL_checknumber(L, 1);
  float y = luaL_checknumber(L, 2);
  gpu_text *texture = (gpu_text *)luaL_checkinteger(L, 3);
  u32 color = 0;
  if (argc == 4) {
    color = luaL_checkinteger(L, 4);
#ifndef SKIP_ERROR_HANDLING
    if (texture->magic != 0x4C545854)
      return luaL_error(L, "attempt to access wrong memory block type");
    if (cur_screen != 1 && cur_screen != 0)
      return luaL_error(L, "you need to call initBlend to use GPU rendering");
#endif
    if (color) {
    }
    // sf2d_draw_texture_blend(texture->tex, x, y, RGBA8((color >> 16) & 0xFF,
    // (color >> 8) & 0xFF, (color)&0xFF, (color >> 24) & 0xFF));
  } else {
#ifndef SKIP_ERROR_HANDLING
    if (texture->magic != 0x4C545854)
      return luaL_error(L, "attempt to access wrong memory block type");
    if (cur_screen != 1 && cur_screen != 0)
      return luaL_error(L, "you need to call initBlend to use GPU rendering");
#endif
    C2D_DrawImageAt(texture->tex[0], x, y, 0.5);
  }
  return 0;
}

static int lua_drawimg_scale(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 5 && argc != 6)
    return luaL_error(L, "wrong number of arguments");
#endif
  float x = luaL_checknumber(L, 1);
  float y = luaL_checknumber(L, 2);
  gpu_text *texture = (gpu_text *)luaL_checkinteger(L, 3);
  float scale_x = luaL_checknumber(L, 4);
  float scale_y = luaL_checknumber(L, 5);
  u32 color;
#ifndef SKIP_ERROR_HANDLING
  if (texture->magic != 0x4C545854)
    return luaL_error(L, "attempt to access wrong memory block type");
  if (cur_screen != 1 && cur_screen != 0)
    return luaL_error(L, "you need to call initBlend to use GPU rendering");
#endif
  if (argc == 6) {
    color = luaL_checkinteger(L, 6);
    if (color) {
    }
    // sf2d_draw_texture_scale_blend(texture->tex, x, y, scale_x, scale_y,
    // RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF, (color >>
    // 24) & 0xFF));
  } else
    C2D_DrawImageAt(texture->tex[0], x, y, 0.5, nullptr, scale_x, scale_y);
  return 0;
}

static int lua_drawimg_rotate(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 4 && argc != 5)
    return luaL_error(L, "wrong number of arguments");
#endif
  float x = luaL_checknumber(L, 1);
  float y = luaL_checknumber(L, 2);
  gpu_text *texture = (gpu_text *)luaL_checkinteger(L, 3);
  float radius = luaL_checknumber(L, 4);
  u32 color;
#ifndef SKIP_ERROR_HANDLING
  if (texture->magic != 0x4C545854)
    return luaL_error(L, "attempt to access wrong memory block type");
  if (cur_screen != 1 && cur_screen != 0)
    return luaL_error(L, "you need to call initBlend to use GPU rendering");
#endif
  if (argc == 5) {
    color = luaL_checkinteger(L, 5);
    if (color) {
    }
    // sf2d_draw_texture_rotate_blend(texture->tex, x, y, radius, RGBA8((color
    // >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF, (color >> 24) & 0xFF));
  } else
    C2D_DrawImageAtRotated(texture->tex[0], x, y, 0.5, radius);
  return 0;
}

static int lua_drawimg_full(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 10 && argc != 11)
    return luaL_error(L, "wrong number of arguments");
#endif
  float x = luaL_checknumber(L, 1);
  float y = luaL_checknumber(L, 2);
  int st_x = luaL_checkinteger(L, 3);
  int st_y = luaL_checkinteger(L, 4);
  float width = luaL_checknumber(L, 5);
  float height = luaL_checknumber(L, 6);
  float radius = luaL_checknumber(L, 7);
  float scale_x = luaL_checknumber(L, 8);
  float scale_y = luaL_checknumber(L, 9);
  gpu_text *texture = (gpu_text *)luaL_checkinteger(L, 10);
  u32 color;
  if (argc == 11)
    color = luaL_checkinteger(L, 11);
#ifndef SKIP_ERROR_HANDLING
  if (texture->magic != 0x4C545854)
    return luaL_error(L, "attempt to access wrong memory block type");
  if (cur_screen != 1 && cur_screen != 0)
    return luaL_error(L, "you need to call initBlend to use GPU rendering");
#endif
  if (argc == 10)
    std::cout << "Not Supported\n";
  // sf2d_draw_texture_part_rotate_scale(texture->tex, x, y, radius, st_x, st_y,
  // width, height, scale_x, scale_y);
  else
    std::cout << "Not Supported\n";
  // sf2d_draw_texture_part_rotate_scale_blend(texture->tex, x, y, radius, st_x,
  // st_y, width, height, scale_x, scale_y, RGBA8((color >> 16) & 0xFF, (color
  // >> 8) & 0xFF, (color)&0xFF, (color >> 24) & 0xFF));
  return 0;
}

static int lua_partial(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 7 && argc != 8)
    return luaL_error(L, "wrong number of arguments");
#endif
  float x = luaL_checknumber(L, 1);
  float y = luaL_checknumber(L, 2);
  int st_x = luaL_checkinteger(L, 3);
  int st_y = luaL_checkinteger(L, 4);
  float width = luaL_checknumber(L, 5);
  float height = luaL_checknumber(L, 6);
  gpu_text *file = (gpu_text *)luaL_checkinteger(L, 7);
  u32 color;
  if (argc == 8)
    color = luaL_checkinteger(L, 8);
#ifndef SKIP_ERROR_HANDLING
  if (file->magic != 0x4C545854)
    return luaL_error(L, "attempt to access wrong memory block type");
  if (cur_screen != 1 && cur_screen != 0)
    return luaL_error(L, "you need to call initBlend to use GPU rendering");
#endif
  if (argc == 8)
    std::cout << "Not Supported\n";
  // sf2d_draw_texture_part_blend(file->tex, x, y, st_x, st_y, width, height,
  // RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color)&0xFF, (color >>
  // 24) & 0xFF));
  else
    std::cout << "Not Supported\n";
  // sf2d_draw_texture_part(file->tex, x, y, st_x, st_y, width, height);
  return 0;
}

static int lua_free(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1)
    return luaL_error(L, "wrong number of arguments");
#endif
  gpu_text *texture = (gpu_text *)luaL_checkinteger(L, 1);
#ifndef SKIP_ERROR_HANDLING
  if (texture->magic != 0x4C545854)
    return luaL_error(L, "attempt to access wrong memory block type");
#endif
  C3D_TexDelete(texture->tex->tex);
  delete[] texture->tex;
  delete[] texture;
  return 0;
}

static int lua_getWidth(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1)
    return luaL_error(L, "wrong number of arguments");
#endif
  gpu_text *src = (gpu_text *)luaL_checkinteger(L, 1);
#ifndef SKIP_ERROR_HANDLING
  if (src->magic != 0x4C545854)
    return luaL_error(L, "attempt to access wrong memory block type");
#endif
  lua_pushinteger(L, src->width);
  return 1;
}

static int lua_getHeight(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1)
    return luaL_error(L, "wrong number of arguments");
#endif
  gpu_text *src = (gpu_text *)luaL_checkinteger(L, 1);
#ifndef SKIP_ERROR_HANDLING
  if (src->magic != 0x4C545854)
    return luaL_error(L, "attempt to access wrong memory block type");
#endif
  lua_pushinteger(L, src->height);
  return 1;
}

static int lua_viewport(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 5)
    return luaL_error(L, "wrong number of arguments");
#endif
  u32 x = luaL_checkinteger(L, 1);
  u32 y = luaL_checkinteger(L, 2);
  u32 w = luaL_checkinteger(L, 3);
  u32 h = luaL_checkinteger(L, 4);
  GPU_SCISSORMODE mode = (GPU_SCISSORMODE)luaL_checkinteger(L, 5);
  // sf2d_set_scissor_test(mode, x, y, w, h);
  return 0;
}

static int lua_pixel2(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if ((argc != 3) && (argc != 4))
    return luaL_error(L, "wrong number of arguments");
#endif
  int x = luaL_checkinteger(L, 1);
  int y = luaL_checkinteger(L, 2);
  int screen = luaL_checkinteger(L, 3);
  int side = 0;
  if (argc == 4)
    side = luaL_checkinteger(L, 4);
#ifndef SKIP_ERROR_HANDLING
  if ((x < 0) || (y < 0))
    return luaL_error(L, "out of bounds");
  if ((screen == 0) && (x > 400))
    return luaL_error(L, "out of framebuffer bounds");
  if ((screen == 1) && (x > 320))
    return luaL_error(L, "out of framebuffer bounds");
  if ((screen <= 1) && (y > 240))
    return luaL_error(L, "out of framebuffer bounds");
  if ((screen > 1) && (((gpu_text *)screen)->magic != 0x4C545854))
    return luaL_error(L, "attempt to access wrong memory block type");
#endif
  if (screen > 1) {
    // u32 color = sf2d_get_pixel(((gpu_text *)screen)->tex, x, y);
    // u32 res = (color >> 16) & 0xFF | (((color >> 8) & 0xFF) << 8) | ((color &
    // 0xFF) << 16) | (((color >> 24) & 0xFF) << 24); lua_pushinteger(L, res);
  } else
    lua_pushinteger(L, GetPixel(x, y, screen, side));
  return 1;
}

// Register our Graphics Functions
static const luaL_Reg Graphics_functions[] = {
    {"init", lua_init},
    {"term", lua_term},
    {"initBlend", lua_refresh},
    {"loadImage", lua_loadimg},
    {"drawImage", lua_drawimg},
    {"drawPartialImage", lua_partial},
    {"drawRotateImage", lua_drawimg_rotate},
    {"drawScaleImage", lua_drawimg_scale},
    {"drawImageExtended", lua_drawimg_full},
    {"fillRect", lua_rect},
    {"fillEmptyRect", lua_emptyrect},
    {"drawCircle", lua_fillcircle},
    {"drawLine", lua_line},
    {"termBlend", lua_end},
    {"flip", lua_flip},
    {"freeImage", lua_free},
    {"getImageWidth", lua_getWidth},
    {"getImageHeight", lua_getHeight},
    {"setViewport", lua_viewport},
    {"getPixel", lua_pixel2},
    {"convertFrom", lua_convert},
    {0, 0}};

void luaGraphics_init(lua_State *L) {
  lua_newtable(L);
  luaL_setfuncs(L, Graphics_functions, 0);
  lua_setglobal(L, "Graphics");
  u8 BORDER = 1;
  u8 CENTER = 3;
  VariableRegister(L, BORDER);
  VariableRegister(L, CENTER);
}