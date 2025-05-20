#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lua.hpp"
#include "utils.h"


void FS_GetSize(fileStream *Handle, u64 *size) {
  if (Handle->isRomfs) {
    FILE *handle = (FILE *)Handle->handle;
    fseek(handle, 0, SEEK_END);
    *size = ftell(handle);
  } else
    FSFILE_GetSize(Handle->handle, size);
}

void FS_Read(fileStream *Handle, u32 *bytesRead, u32 init, void *buffer,
             u32 size) {
  if (Handle->isRomfs) {
    FILE *handle = (FILE *)Handle->handle;
    fseek(handle, init, SEEK_SET);
    *bytesRead = fread(buffer, 1, size, handle);
  } else {
    Result ret = FSFILE_Read(Handle->handle, bytesRead, init, buffer, size);
  }
}

void FS_Close(fileStream *Handle) {
  if (Handle->isRomfs) {
    FILE *handle = (FILE *)Handle->handle;
    fclose(handle);
  } else {
    Result ret = FSFILE_Close(Handle->handle);
    svcCloseHandle(Handle->handle);
  }
}

void utf2ascii(char *dst, u16 *src) {
  if (!src || !dst) return;
  while (*src) *(dst++) = (*(src++)) & 0xFF;
  *dst = 0x00;
}

void ascii2utf(u16 *dst, char *src) {
  if (!src || !dst) return;
  while (*src) *(dst++) = (*src++);
  *dst = 0x00;
}

u32 Endian_UInt32_Conversion(u32 value) {
  return ((value >> 24) & 0x000000FF) | ((value >> 8) & 0x0000FF00) |
         ((value << 8) & 0x00FF0000) | ((value << 24) & 0xFF000000);
}

u16 Endian_UInt16_Conversion(u16 value) {
  return (u16)(((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00));
}

void int2float(u32 color, float *r, float *g, float *b, float *a) {
  u32 b1 = color & 0xFF;
  u32 g1 = (color >> 8) & 0xFF;
  u32 r1 = (color >> 16) & 0xFF;
  u32 a1 = (color >> 24) & 0xFF;
  *r = float(r1) / 255.0f;
  *b = float(b1) / 255.0f;
  *g = float(g1) / 255.0f;
  *a = float(a1) / 255.0f;
}

void float2int(color *base, u32 *result) {
  u32 b = int(base->b * 255.0f);
  u32 g = int(base->g * 255.0f);
  u32 r = int(base->r * 255.0f);
  u32 a = int(base->a * 255.0f);
  *result = b | (g << 8) | (r << 16) | (a << 24);
}

u32 ARGB2RGBA(u32 color) {
  u32 a = (color >> 24);
  u32 r = ((color << 8) >> 24);
  u32 g = ((color << 16) >> 24);
  u32 b = ((color << 24) >> 24);
  return a | (b << 8) | (g << 16) | (r << 24);
}

void RBswap(u32 *color) {
  u8 *ptr = (u8 *)color;
  u8 swap = ptr[0];
  ptr[0] = ptr[2];
  ptr[2] = swap;
}

// Grabbed from Citra Emulator (citra/src/video_core/utils.h)
u32 morton_interleave(u32 x, u32 y) {
  u32 i = (x & 7) | ((y & 7) << 8);  // ---- -210
  i = (i ^ (i << 2)) & 0x1313;       // ---2 --10
  i = (i ^ (i << 1)) & 0x1515;       // ---2 -1-0
  i = (i | (i >> 7)) & 0x3F;
  return i;
}

// Grabbed from Citra Emulator (citra/src/video_core/utils.h)
u32 get_morton_offset(u32 x, u32 y, u32 bytes_per_pixel) {
  u32 i = morton_interleave(x, y);
  unsigned int offset = (x & ~7) << 3;
  return (i + offset) * bytes_per_pixel;
}

void *luaL_checkbuffer(lua_State *L, int argv) {
  if (lua_type(L, argv) == LUA_TNUMBER)
    (void *)luaL_checkinteger(L, argv);
  else
    return (void *)luaL_checkstring(L, argv);
  return nullptr;
}