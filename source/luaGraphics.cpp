/*----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#------  This File is Part Of : ----------------------------------------------------------------------------------------#
#------- _  -------------------  ______   _   --------------------------------------------------------------------------#
#------ | | ------------------- (_____ \ | |  --------------------------------------------------------------------------#
#------ | | ---  _   _   ____    _____) )| |  ____  _   _   ____   ____   ----------------------------------------------#
#------ | | --- | | | | / _  |  |  ____/ | | / _  || | | | / _  ) / ___)  ----------------------------------------------#
#------ | |_____| |_| |( ( | |  | |      | |( ( | || |_| |( (/ / | |  --------------------------------------------------#
#------ |_______)\____| \_||_|  |_|      |_| \_||_| \__  | \____)|_|  --------------------------------------------------#
#------------------------------------------------- (____/  -------------------------------------------------------------#
#------------------------   ______   _   -------------------------------------------------------------------------------#
#------------------------  (_____ \ | |  -------------------------------------------------------------------------------#
#------------------------   _____) )| | _   _   ___   ------------------------------------------------------------------#
#------------------------  |  ____/ | || | | | /___)  ------------------------------------------------------------------#
#------------------------  | |      | || |_| ||___ |  ------------------------------------------------------------------#
#------------------------  |_|      |_| \____|(___/   ------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Licensed under the GPL License --------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Copyright (c) Nanni <lpp.nanni@gmail.com> ---------------------------------------------------------------------------#
#- Copyright (c) Rinnegatamante <rinnegatamante@gmail.com> -------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Credits : -----------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Smealum for ctrulib and ftpony src ----------------------------------------------------------------------------------#
#- StapleButter for debug font -----------------------------------------------------------------------------------------#
#- Lode Vandevenne for lodepng -----------------------------------------------------------------------------------------#
#- Jean-loup Gailly and Mark Adler for zlib ----------------------------------------------------------------------------#
#- xerpi for sf2dlib ---------------------------------------------------------------------------------------------------#
#- Special thanks to Aurelio for testing, bug-fixing and various help with codes and implementations -------------------#
#-----------------------------------------------------------------------------------------------------------------------*/
#include <3ds.h>
#include <string.h>
#include "include/luaplayer.h"
#include "include/graphics/Graphics.h"
#include "include/utils.h"
#define stringify(str) #str
#define VariableRegister(lua, value) do { lua_pushinteger(lua, value); lua_setglobal (lua, stringify(value)); } while(0)

Target screentg = TOP;
C3D_RenderTarget *curtg;
bool rfshcl = false;

bool c3disinit = false;
static int lua_init(lua_State *L) {
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 0) return luaL_error(L, "wrong number of arguments");	
	#endif
	if (!c3disinit){ C3D_Init(C3D_DEFAULT_CMDBUF_SIZE); c3disinit = true;}
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	TG_Top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	TG_TopRight = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);
	TG_Bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	return 0;
}

static int lua_term(lua_State *L) {
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 0) return luaL_error(L, "wrong number of arguments");
	#endif
	
	C2D_Fini();
	C3D_Fini();
	return 0;
}

static int lua_refresh(lua_State *L) {
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if ((argc != 1) && (argc != 2))  return luaL_error(L, "wrong number of arguments");
	#endif
	Target tg = (Target)luaL_checkinteger(L,1);
	int side=0;
	if (argc == 2) side = luaL_checkinteger(L,2);
	screentg = tg;
	switch (tg)
	{
	case TOP:
		curtg = TG_Top;
		break;
	case BOTTOM:
		curtg = TG_Bottom;
		break;
	default:
		curtg = TG_Top;
		break;
	}
	rfshcl = true;
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_SceneBegin(curtg);
	return 0;
}

static int lua_end(lua_State *L) {
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 0) return luaL_error(L, "wrong number of arguments");
	#endif
	C3D_FrameEnd(0);
	return 0;
}

static int lua_flip(lua_State *L) {
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 0) return luaL_error(L, "wrong number of arguments");
	#endif
	luaL_error(L, "This Function is useless in lpp-3ds-next");
	return 0;
}

static int lua_rect(lua_State *L) {
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 5) return luaL_error(L, "wrong number of arguments");
	#endif
	float x1 = luaL_checknumber(L,1);
	float x2 = luaL_checknumber(L,2);
	float y1 = luaL_checknumber(L,3);
	float y2 = luaL_checknumber(L,4);
	float radius = 0;
	
	u32 color = luaL_checkinteger(L,5);
	#ifndef SKIP_ERROR_HANDLING
	if (rfshcl) return luaL_error(L, "you need to call initBlend to use GPU rendering");
	#endif
	C2D_DrawRectSolid(x1, y1, 0.5f,  x2, y2, color);
	return 0;
}

static int lua_fillcircle(lua_State *L) {
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 4) return luaL_error(L, "wrong number of arguments");
	#endif
	float x = luaL_checknumber(L,1);
	float y = luaL_checknumber(L,2);
	int radius = luaL_checkinteger(L,3);
	u32 color = luaL_checkinteger(L,4);
	#ifndef SKIP_ERROR_HANDLING
	if (rfshcl) return luaL_error(L, "you need to call initBlend to use GPU rendering");
	#endif
	C2D_DrawCircleSolid(x, y, 0.5, radius, color);
	return 0;
}


static int lua_line(lua_State *L) {
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 5) return luaL_error(L, "wrong number of arguments");
	#endif
	float x1 = luaL_checknumber(L,1);
	float x2 = luaL_checknumber(L,2);
	float y1 = luaL_checknumber(L,3);
	float y2 = luaL_checknumber(L,4);
	#ifndef SKIP_ERROR_HANDLING
	if (rfshcl) return luaL_error(L, "you need to call initBlend to use GPU rendering");
	#endif
	u32 color = luaL_checkinteger(L,5);
	C2D_DrawLine(x1, y1, color, x2, y2, color, 1, 1);
	return 0;
}

static int lua_emptyrect(lua_State *L) {
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 5) return luaL_error(L, "wrong number of arguments");
	#endif
	float x1 = luaL_checknumber(L,1);
	float x2 = luaL_checknumber(L,2);
	float y1 = luaL_checknumber(L,3);
	float y2 = luaL_checknumber(L,4);
	if (x2 < x1){
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	if (y2 < y1){
		int tmp = y2;
		y2 = y1;
		y1 = tmp;
	}
	#ifndef SKIP_ERROR_HANDLING
	if (rfshcl) return luaL_error(L, "you need to call initBlend to use GPU rendering");
	#endif
	u32 color = luaL_checkinteger(L,5);
	C2D_DrawLine(x1, y1, color, x2, y2, color, 1, 1);
	return 0;
}

static int lua_loadimg(lua_State *L){
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 1) return luaL_error(L, "wrong number of arguments");
	#endif
	char* text = (char*)(luaL_checkstring(L, 1));
	fileStream fileHandle;
	u32 bytesRead;
	u16 magic;
	u64 long_magic;
	if (strncmp("romfs:/",text,7) == 0){
		fileHandle.isRomfs = true;
		FILE* handle = fopen(text,"r");
		#ifndef SKIP_ERROR_HANDLING
		if (handle == NULL) return luaL_error(L, "file doesn't exist.");
		#endif
		fileHandle.handle = (u32)handle;
	}else{
		fileHandle.isRomfs = false;
		FS_Path filePath = fsMakePath(PATH_ASCII, text);
		FS_Path m_path = (FS_Path){PATH_EMPTY, 1, (u8*)""};
		FS_Archive script;
		FSUSER_OpenArchive(&script, ARCHIVE_SDMC, m_path);
		Result ret = FSUSER_OpenFileDirectly( &fileHandle.handle, ARCHIVE_SDMC, filePath, filePath, FS_OPEN_READ, 0x00000000);
		#ifndef SKIP_ERROR_HANDLING
		if (ret) return luaL_error(L, "file doesn't exist.");
		#endif
	}
	FS_Read(&fileHandle, &bytesRead, 0, &magic, 2);
	Bitmap* bitmap;
	if (magic == 0x5089){
		FS_Read(&fileHandle, &bytesRead, 0, &long_magic, 8);
		FS_Close(&fileHandle);
		if (long_magic == 0x0A1A0A0D474E5089) bitmap = decodePNGfile(text);
	}else if (magic == 0x4D42){
		FS_Close(&fileHandle);
		bitmap = decodeBMPfile(text);
	}else if (magic == 0xD8FF){
		FS_Close(&fileHandle);
		bitmap = decodeJPGfile(text);
	}
	#ifndef SKIP_ERROR_HANDLING
	if(!bitmap) return luaL_error(L, "Error loading image");
	#endif
	if (bitmap->bitperpixel == 24){
		int length = (bitmap->width * bitmap->height) << 2;
		u8* real_pixels = (u8*)malloc(length);
		int i = 0;
		int z = 0;
		while (i < length){
			real_pixels[i] = bitmap->pixels[z];
			real_pixels[i+1] = bitmap->pixels[z+1];
			real_pixels[i+2] = bitmap->pixels[z+2];
			real_pixels[i+3] = 0xFF;
			i = i + 4;
			z = z + 3;
		}
		free(bitmap->pixels);
		bitmap->pixels = real_pixels;
	}
	C3D_Tex *tex;
	Tex3DS_SubTexture *subtex;
	C3D_TexInit(tex, (u16)bitmap->width, (u16)bitmap->height, GPU_RGBA8);
	C3D_TexSetFilter(tex, GPU_LINEAR, GPU_LINEAR);
	subtex = new Tex3DS_SubTexture({(u16)bitmap->width, (u16)bitmap->height, 0.0f, 1.0f, bitmap->width / 1024.0f, 1.0f - (bitmap->height / 1024.0f)});
	tex->border = 0xFFFFFFFF;
	C3D_TexSetWrap(tex, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);
	for (u32 x = 0; x < bitmap->width && x < 1024; x++) {
		for (u32 y = 0; y < bitmap->height && y < 1024; y++) {
			const u32 dstPos = ((((y >> 3) * (1024 >> 3) + (x >> 3)) << 6) +
								((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) |
								((x & 4) << 2) | ((y & 4) << 3))) * 4;

			const u32 srcPos = (y * bitmap->width + x) * 4;
			((uint8_t *)tex->data)[dstPos + 0] = bitmap->pixels[srcPos + 3];
			((uint8_t *)tex->data)[dstPos + 1] = bitmap->pixels[srcPos + 2];
			((uint8_t *)tex->data)[dstPos + 2] = bitmap->pixels[srcPos + 1];
			((uint8_t *)tex->data)[dstPos + 3] = bitmap->pixels[srcPos + 0];
		}
	}
	
	gpu_text* result = (gpu_text*)malloc(sizeof(gpu_text));
	result->magic = 0x4C545854;
	result->tex = tex;
	result->subtex = subtex;
	result->width = bitmap->width;
	result->height = bitmap->height;
	free(bitmap->pixels);
	free(bitmap);
	lua_pushinteger(L, (u32)(result));
	return 1;
}

static int lua_convert(lua_State *L){
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 1) return luaL_error(L, "wrong number of arguments");
	#endif
	Bitmap* bitmap = (Bitmap*)(luaL_checkinteger(L, 1));
	#ifndef SKIP_ERROR_HANDLING
	if (bitmap->magic != 0x4C494D47) return luaL_error(L, "attempt to access wrong memory block type");
	#endif
	u8* real_pixels;
	u8* flipped = (u8*)malloc(bitmap->width * bitmap->height * (bitmap->bitperpixel >> 3));
	flipped = flipBitmap(flipped, bitmap);
	int length = (bitmap->width * bitmap->height) << 2;
	if (bitmap->bitperpixel == 24){		
		real_pixels = (u8*)malloc(length);
		int i = 0;
		int z = 0;
		while (i < length){
			real_pixels[i] = flipped[z+2];
			real_pixels[i+1] = flipped[z+1];
			real_pixels[i+2] = flipped[z];
			real_pixels[i+3] = 0xFF;
			i = i + 4;
			z = z + 3;
		}
		free(flipped);
	}else{
		real_pixels = flipped;
		int i = 0;
		while (i < length){
			u8 tmp = real_pixels[i+2];
			real_pixels[i+2] = real_pixels[i];
			real_pixels[i] = tmp;
			i = i + 4;
		}
	}
	C3D_Tex *tex;
	Tex3DS_SubTexture *subtex;
	C3D_TexInit(tex, (u16)bitmap->width, (u16)bitmap->height, GPU_RGBA8);
	C3D_TexSetFilter(tex, GPU_LINEAR, GPU_LINEAR);
	subtex = new Tex3DS_SubTexture({(u16)bitmap->width, (u16)bitmap->height, 0.0f, 1.0f, bitmap->width / 1024.0f, 1.0f - (bitmap->height / 1024.0f)});
	tex->border = 0xFFFFFFFF;
	C3D_TexSetWrap(tex, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);
	for (u32 x = 0; x < bitmap->width && x < 1024; x++) {
		for (u32 y = 0; y < bitmap->height && y < 1024; y++) {
			const u32 dstPos = ((((y >> 3) * (1024 >> 3) + (x >> 3)) << 6) +
								((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) |
								((x & 4) << 2) | ((y & 4) << 3))) * 4;

			const u32 srcPos = (y * bitmap->width + x) * 4;
			((uint8_t *)tex->data)[dstPos + 0] = bitmap->pixels[srcPos + 3];
			((uint8_t *)tex->data)[dstPos + 1] = bitmap->pixels[srcPos + 2];
			((uint8_t *)tex->data)[dstPos + 2] = bitmap->pixels[srcPos + 1];
			((uint8_t *)tex->data)[dstPos + 3] = bitmap->pixels[srcPos + 0];
		}
	}
	
	gpu_text* result = (gpu_text*)malloc(sizeof(gpu_text));
	result->magic = 0x4C545854;
	result->tex = tex;
	result->subtex = subtex;
	result->width = bitmap->width;
	result->height = bitmap->height;
	free(real_pixels);
	lua_pushinteger(L, (u32)(result));
	return 1;
}

static int lua_drawimg(lua_State *L){
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 3 && argc != 4) return luaL_error(L, "wrong number of arguments");
	#endif
	float x = luaL_checknumber(L,1);
	float y = luaL_checknumber(L,2);
	gpu_text* texture = (gpu_text*)luaL_checkinteger(L,3);
	C2D_Image img;
	img.tex = texture->tex;
	img.subtex = texture->subtex;
	C2D_ImageTint tnt;
	
	u32 color = 0;
	if (argc == 4){ 
		color = luaL_checkinteger(L,4);
		#ifndef SKIP_ERROR_HANDLING
		if (texture->magic != 0x4C545854) return luaL_error(L, "attempt to access wrong memory block type");
		if (rfshcl) return luaL_error(L, "you need to call initBlend to use GPU rendering");
		#endif
		tnt.corners->color = color;
		C2D_DrawImageAt(img, x, y, 0.5f, tnt);
	}else{
		#ifndef SKIP_ERROR_HANDLING
		if (texture->magic != 0x4C545854) return luaL_error(L, "attempt to access wrong memory block type");
		if (rfshcl) return luaL_error(L, "you need to call initBlend to use GPU rendering");
		#endif
		C2D_DrawImageAt(img, x, y, 0.5f, nullptr);
	}
	return 0;
}

static int lua_drawimg_scale(lua_State *L){
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 5 && argc != 6) return luaL_error(L, "wrong number of arguments");
	#endif
	float x = luaL_checknumber(L,1);
	float y = luaL_checknumber(L,2);
	gpu_text* texture = (gpu_text*)luaL_checkinteger(L,3);
	float scale_x = luaL_checknumber(L,4);
	float scale_y = luaL_checknumber(L,5);
	u32 color;
	C2D_Image img;
	img.tex = texture->tex;
	img.subtex = texture->subtex;
	C2D_ImageTint tnt;
	#ifndef SKIP_ERROR_HANDLING
	if (texture->magic != 0x4C545854) return luaL_error(L, "attempt to access wrong memory block type");
	if (rfshcl) return luaL_error(L, "you need to call initBlend to use GPU rendering");
	#endif
	if (argc == 6){
		color = luaL_checkinteger(L,6);
		tnt.corners->color = color;
		C2D_DrawImageAt(img, x, y, 0.5f, tnt);
	}else C2D_DrawImageAt(img, x, y, 0.5f, nullptr);
	return 0;
}

static int lua_drawimg_rotate(lua_State *L){
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 4 && argc != 5) return luaL_error(L, "wrong number of arguments");
	#endif
	float x = luaL_checknumber(L,1);
	float y = luaL_checknumber(L,2);
	gpu_text* texture = (gpu_text*)luaL_checkinteger(L,3);
	float radius = luaL_checknumber(L,4);
	u32 color;
	C2D_Image img;
	img.tex = texture->tex;
	img.subtex = texture->subtex;
	C2D_ImageTint tnt;
	#ifndef SKIP_ERROR_HANDLING
	if (texture->magic != 0x4C545854) return luaL_error(L, "attempt to access wrong memory block type");
	if (rfshcl) return luaL_error(L, "you need to call initBlend to use GPU rendering");
	#endif
	if (argc == 5){
		color = luaL_checkinteger(L,5);
		tnt.corners->color = color;
		C2D_DrawImageAtRotated(img, x, y, 1.0f, radius, tnt);
	}else C2D_DrawImageAtRotated(img, x, y, 1.0f, radius, nullptr);
	return 0;
}

static int lua_drawimg_full(lua_State *L){
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 10 && argc != 11) return luaL_error(L, "wrong number of arguments");
	#endif
	float x = luaL_checknumber(L,1);
	float y = luaL_checknumber(L,2);
	int st_x = luaL_checkinteger(L, 3);
	int st_y = luaL_checkinteger(L, 4);
	float width = luaL_checknumber(L, 5);
	float height = luaL_checknumber(L, 6);
	float radius = luaL_checknumber(L, 7);
	float scale_x = luaL_checknumber(L, 8);
	float scale_y = luaL_checknumber(L, 9);
	gpu_text* texture = (gpu_text*)luaL_checkinteger(L, 10);
	u32 color;
	if (argc == 11) color = luaL_checkinteger(L, 11);
	#ifndef SKIP_ERROR_HANDLING
	if (texture->magic != 0x4C545854) return luaL_error(L, "attempt to access wrong memory block type");
	if (rfshcl) return luaL_error(L, "you need to call initBlend to use GPU rendering");
	#endif
	if (argc == 10) //sf2d_draw_texture_part_rotate_scale(texture->tex, x, y, radius, st_x, st_y, width, height, scale_x, scale_y);
	else //sf2d_draw_texture_part_rotate_scale_blend(texture->tex, x, y, radius, st_x, st_y, width, height, scale_x, scale_y, RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color) & 0xFF, (color >> 24) & 0xFF));
	return 0;
}

static int lua_partial(lua_State *L){
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 7 && argc != 8) return luaL_error(L, "wrong number of arguments");
	#endif
	float x = luaL_checknumber(L,1);
	float y = luaL_checknumber(L,2);
	int st_x = luaL_checkinteger(L, 3);
	int st_y = luaL_checkinteger(L, 4);
	float width = luaL_checknumber(L, 5);
	float height = luaL_checknumber(L, 6);
	gpu_text* file = (gpu_text*)luaL_checkinteger(L, 7);
	u32 color;
	if (argc == 8) color = luaL_checkinteger(L, 8);
	#ifndef SKIP_ERROR_HANDLING
	if (file->magic != 0x4C545854) return luaL_error(L, "attempt to access wrong memory block type");
	if (rfshcl) return luaL_error(L, "you need to call initBlend to use GPU rendering");
	#endif
	if (argc == 8) //sf2d_draw_texture_part_blend(file->tex, x, y, st_x, st_y, width, height, RGBA8((color >> 16) & 0xFF, (color >> 8) & 0xFF, (color) & 0xFF, (color >> 24) & 0xFF));
	else //sf2d_draw_texture_part(file->tex, x, y, st_x, st_y, width, height);
	return 0;
}

static int lua_free(lua_State *L){
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 1) return luaL_error(L, "wrong number of arguments");
	#endif
	gpu_text* texture = (gpu_text*)luaL_checkinteger(L,1);
	#ifndef SKIP_ERROR_HANDLING
	if (texture->magic != 0x4C545854) return luaL_error(L, "attempt to access wrong memory block type");
	#endif
	
	free(texture);
	return 0;
}

static int lua_getWidth(lua_State *L){
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 1) return luaL_error(L, "wrong number of arguments");
	#endif
	gpu_text* src = (gpu_text*)luaL_checkinteger(L, 1);
	#ifndef SKIP_ERROR_HANDLING
	if (src->magic != 0x4C545854) return luaL_error(L, "attempt to access wrong memory block type");
	#endif
	lua_pushinteger(L,src->width);
	return 1;
}

static int lua_getHeight(lua_State *L){
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 1) return luaL_error(L, "wrong number of arguments");
	#endif
	gpu_text* src = (gpu_text*)luaL_checkinteger(L, 1);
	#ifndef SKIP_ERROR_HANDLING
	if (src->magic != 0x4C545854) return luaL_error(L, "attempt to access wrong memory block type");
	#endif
	lua_pushinteger(L,src->height);
	return 1;
}

static int lua_viewport(lua_State *L){
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if (argc != 5) return luaL_error(L, "wrong number of arguments");
	#endif
	u32 x = luaL_checkinteger(L,1);
	u32 y = luaL_checkinteger(L,2);
	u32 w = luaL_checkinteger(L,3);
	u32 h = luaL_checkinteger(L,4);
	GPU_SCISSORMODE mode = (GPU_SCISSORMODE)luaL_checkinteger(L,5);
	C2D_ViewScale(w, h);
	//sf2d_set_scissor_test(mode, x, y, w, h);
	return 0;
}

static int lua_pixel2(lua_State *L){
	int argc = lua_gettop(L);
	#ifndef SKIP_ERROR_HANDLING
	if ((argc != 3) && (argc != 4)) return luaL_error(L, "wrong number of arguments");
	#endif
	int x = luaL_checkinteger(L,1);
	int y = luaL_checkinteger(L,2);
	int screen = luaL_checkinteger(L,3);
	int side=0;
	if (argc == 4) side = luaL_checkinteger(L,4);
	#ifndef SKIP_ERROR_HANDLING
	if ((x < 0) || (y < 0)) return luaL_error(L, "out of bounds");
	if ((screen == 0) && (x > 400)) return luaL_error(L, "out of framebuffer bounds");
	if ((screen == 1) && (x > 320)) return luaL_error(L, "out of framebuffer bounds");
	if ((screen <= 1) && (y > 240)) return luaL_error(L, "out of framebuffer bounds");
	if ((screen > 1) && (((gpu_text*)screen)->magic != 0x4C545854)) return luaL_error(L, "attempt to access wrong memory block type");
	#endif
	if (screen > 1){
		u32 color = 0xFFFFFFFF;
		
		u32 res = (color >> 16) & 0xFF | (((color >> 8) & 0xFF) << 8) | ((color & 0xFF) << 16) | (((color >> 24) & 0xFF) << 24);
		lua_pushinteger(L,res);
	}else lua_pushinteger(L,GetPixel(x,y,screen,side));
	return 1;
}

//Register our Graphics Functions
static const luaL_Reg Graphics_functions[] = {
  {"init",                  lua_init},
  {"term",                  lua_term},
  {"initBlend",             lua_refresh},
  {"loadImage",             lua_loadimg},
  {"drawImage",             lua_drawimg},
  {"drawPartialImage",      lua_partial},
  {"drawRotateImage",       lua_drawimg_rotate},
  {"drawScaleImage",        lua_drawimg_scale},
  {"drawImageExtended",     lua_drawimg_full},
  {"fillRect",              lua_rect},
  {"fillEmptyRect",         lua_emptyrect},
  {"drawCircle",            lua_fillcircle},
  {"drawLine",              lua_line},
  {"termBlend",             lua_end},
  {"flip",                  lua_flip},
  {"freeImage",             lua_free},
  {"getImageWidth",         lua_getWidth},
  {"getImageHeight",        lua_getHeight}, 
  {"setViewport",           lua_viewport}, 
  {"getPixel",              lua_pixel2}, 
  {"convertFrom",           lua_convert},
  {0, 0}
};

void luaGraphics_init(lua_State *L) {
	lua_newtable(L);
	luaL_setfuncs(L, Graphics_functions, 0);
	lua_setglobal(L, "Graphics");
	u8 BORDER = 1;
	u8 CENTER = 3;
	Target Top = TOP;
	Target Bottom = BOTTOM;
	VariableRegister(L, Top);
	VariableRegister(L, Bottom);
	VariableRegister(L,BORDER);
	VariableRegister(L,CENTER);
}