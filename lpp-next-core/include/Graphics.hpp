#include <3ds.h>
#include <citro2d.h>

struct Bitmap {
  u32 magic;
  u8 *pixels;
  int width;
  int height;
  u16 bitperpixel;
};
struct gpu_text {
  u32 magic;
  u16 width;
  u16 height;
  C2D_Image *tex;
};
struct Console {
  u32 magic;
  int screen;
  char text[1500];
};
extern u8 *TopLFB;
extern u8 *TopRFB;
extern u8 *BottomFB;
Bitmap *LoadBitmap(char *fname);
void printJpg(unsigned char *in, u64 size, u8 *framebuffer);
void DrawRGB565Pixel(u8 *dst, u16 x, u16 y, u16 v);
void DrawRGB565Screen(u8 *dst, u16 *pic);
void putPixel565(u8 *dst, u8 x, u8 y, u16 v);
void PrintScreenBitmap(int xp, int yp, Bitmap *result, int screen, int side);
void PrintPartialScreenBitmap(int xp, int yp, int st_x, int st_y, int width,
                              int height, Bitmap *result, int screen, int side);
void PrintPartialImageBitmap(int xp, int yp, int st_x, int st_y, int width,
                             int height, Bitmap *result, int screen);
void PrintPartialGpuBitmap(int xp, int yp, int st_x, int st_y, int width,
                           int height, Bitmap *result, int screen);
void PrintImageBitmap(int xp, int yp, Bitmap *result, int screen);
void PrintGpuBitmap(int xp, int yp, Bitmap *result, int screen);
u8 *flipBitmap(u8 *flip_bitmap, Bitmap *result);
void DrawImagePixel(int x, int y, u32 color, Bitmap *screen);
void DrawAlphaImagePixel(int x, int y, u32 color, Bitmap *screen);
void DrawPixel(u8 *screen, int x, int y, u32 color);
void DrawAlphaPixel(u8 *screen, int x, int y, u32 color);
void DrawScreenText(int x, int y, char *str, u32 color, int screen, int side);
void DrawAlphaScreenText(int x, int y, char *str, u32 color, int screen,
                         int side);
void DrawImageText(int x, int y, char *str, u32 color, int screen);
void DrawGpuText(int x, int y, char *str, u32 color, int screen);
void DrawAlphaImageText(int x, int y, char *str, u32 color, int screen);
void Draw32bppImagePixel(int x, int y, u32 color, Bitmap *screen);
void Fill32bppImageEmptyRect(int x1, int x2, int y1, int y2, u32 color,
                             int screen);
void Fill32bppImageRect(int x1, int x2, int y1, int y2, u32 color, int screen);
void FillGpuRect(int x1, int x2, int y1, int y2, u32 color, int screen);
void Draw32bppImageText(int x, int y, char *str, u32 color, int screen);
void RefreshScreen();
void DebugOutput(char *str);
int ConsoleOutput(Console *console);
void FillImageRect(int x1, int x2, int y1, int y2, u32 color, int screen);
void FillAlphaImageRect(int x1, int x2, int y1, int y2, u32 color, int screen);
void FillScreenRect(int x1, int x2, int y1, int y2, u32 color, int screen,
                    int side);
void FillAlphaScreenRect(int x1, int x2, int y1, int y2, u32 color, int screen,
                         int side);
void FillScreenEmptyRect(int x1, int x2, int y1, int y2, u32 color, int screen,
                         int side);
void FillAlphaScreenEmptyRect(int x1, int x2, int y1, int y2, u32 color,
                              int screen, int side);
void FillImageEmptyRect(int x1, int x2, int y1, int y2, u32 color, int screen);
void FillGpuEmptyRect(int x1, int x2, int y1, int y2, u32 color, int screen);
void FillAlphaImageEmptyRect(int x1, int x2, int y1, int y2, u32 color,
                             int screen);
void ClearScreen(int screen);
void DrawScreenLine(int x0, int y0, int x1, int y1, u32 color, int screen,
                    int side);
void DrawAlphaScreenLine(int x0, int y0, int x1, int y1, u32 color, int screen,
                         int side);
void Draw32bppImageLine(int x0, int y0, int x1, int y1, u32 color, int screen);
void DrawImageLine(int x0, int y0, int x1, int y1, u32 color, int screen);
void DrawGpuLine(int x0, int y0, int x1, int y1, u32 color, int screen);
void DrawAlphaImageLine(int x0, int y0, int x1, int y1, u32 color, int screen);
u32 GetPixel(int x, int y, int screen, int side);
u32 GetImagePixel(int x, int y, Bitmap *screen);
Bitmap *loadPng(const char *filename);
Bitmap *OpenJPG(const char *filename);
Bitmap *decodeJpg(unsigned char *in, u64 size);
void RAW2FB(int xp, int yp, Bitmap *result, int screen, int side);
void saveJpg(char *filename, u32 *pixels, u32 width, u32 height);
u32 *toJpg(u32 *size, u32 *pixels, u32 width, u32 height);
Bitmap *decodePNGfile(const char *filename);
Bitmap *decodeJPGfile(const char *filename);
Bitmap *decodeBMPfile(const char *filename);