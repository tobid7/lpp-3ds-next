/*
 * hw_graphics.h
 *
 *  Created on: Sep 11, 2015
 *      Author: jbr373
 */

#ifndef HW_GRAPHICS_H_
#define HW_GRAPHICS_H_

#include <3ds.h>

class HW_Graphics {
 public:
  HW_Graphics();
  virtual ~HW_Graphics();

  void DrawKeyboard(u8 &KeyboadState, s16 &T_X, s16 &T_Y, bool &isShift,
                    bool &isCaps, u8 &Key);

 private:
  void DrawKeyShadow(u8 &Key, bool &isShift, bool &isCaps);

  // Smealum Stuff
  void gfxDrawSprite(gfxScreen_t screen, gfx3dSide_t side, u8 *spriteData,
                     u16 width, u16 height, s16 x, s16 y);
  void gfxDrawSpriteAlpha(gfxScreen_t screen, gfx3dSide_t side, u8 *spriteData,
                          u16 width, u16 height, s16 x, s16 y);
};

#endif /* HW_GRAPHICS_H_ */