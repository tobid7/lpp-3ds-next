/*
 * hw_helper.cpp
 *
 *  Created on: Sep 10, 2015
 *      Author: jbr373
 */

#include <3ds.h>

#include <string>

#include "hw_controls.h"


enum Keys {
  HBKB_KEY_NONE = 0,

  HBKB_KEY_A = 1,
  HBKB_KEY_B = 2,
  HBKB_KEY_C = 3,
  HBKB_KEY_D = 4,
  HBKB_KEY_E = 5,
  HBKB_KEY_F = 6,
  HBKB_KEY_G = 7,
  HBKB_KEY_H = 8,
  HBKB_KEY_I = 9,
  HBKB_KEY_J = 10,
  HBKB_KEY_K = 11,
  HBKB_KEY_L = 12,
  HBKB_KEY_M = 13,
  HBKB_KEY_N = 14,
  HBKB_KEY_O = 15,
  HBKB_KEY_P = 16,
  HBKB_KEY_Q = 17,
  HBKB_KEY_R = 18,
  HBKB_KEY_S = 19,
  HBKB_KEY_T = 20,
  HBKB_KEY_U = 21,
  HBKB_KEY_V = 22,
  HBKB_KEY_W = 23,
  HBKB_KEY_X = 24,
  HBKB_KEY_Y = 25,
  HBKB_KEY_Z = 26,

  HBKB_KEY_1 = 53,
  HBKB_KEY_2 = 54,
  HBKB_KEY_3 = 55,
  HBKB_KEY_4 = 56,
  HBKB_KEY_5 = 57,
  HBKB_KEY_6 = 58,
  HBKB_KEY_7 = 59,
  HBKB_KEY_8 = 60,
  HBKB_KEY_9 = 61,
  HBKB_KEY_0 = 62,

  HBKB_KEY_SPACE = 63,

  // Special - Row 0
  HBKB_KEY_SPECIAL_A = 66,  // !
  HBKB_KEY_SPECIAL_B = 67,  // "
  HBKB_KEY_SPECIAL_C = 68,  // §
  HBKB_KEY_SPECIAL_D = 69,  // $
  HBKB_KEY_SPECIAL_E = 70,  // %
  HBKB_KEY_SPECIAL_F = 71,  // &
  HBKB_KEY_SPECIAL_G = 72,  // /
  HBKB_KEY_SPECIAL_H = 73,  // (
  HBKB_KEY_SPECIAL_I = 74,  // )
  HBKB_KEY_SPECIAL_J = 75,  // ?

  // Special - Row 1
  HBKB_KEY_SPECIAL_K = 76,  // *
  HBKB_KEY_SPECIAL_L = 77,  // '
  HBKB_KEY_SPECIAL_M = 78,  // <
  HBKB_KEY_SPECIAL_N = 79,  // >
  HBKB_KEY_SPECIAL_O = 80,  // +
  HBKB_KEY_SPECIAL_P = 81,  // [
  HBKB_KEY_SPECIAL_Q = 82,  // ]
  HBKB_KEY_SPECIAL_R = 83,  // {
  HBKB_KEY_SPECIAL_S = 84,  // }

  // Special - Row 2
  HBKB_KEY_SPECIAL_T = 85,  // ,
  HBKB_KEY_SPECIAL_U = 86,  // ;
  HBKB_KEY_SPECIAL_V = 87,  // .
  HBKB_KEY_SPECIAL_W = 88,  // :
  HBKB_KEY_SPECIAL_X = 89,  // -
  HBKB_KEY_SPECIAL_Y = 90,  // _
  HBKB_KEY_SPECIAL_Z = 99,  // =

  HBKB_KEY_ENTER = 91,
  HBKB_KEY_BACK = 92,
  HBKB_KEY_SHIFT = 93,
  HBKB_KEY_CAPS = 94,
  HBKB_KEY_CHANGEKEYS = 95,
  HBKB_KEY_CANCEL = 96
};

enum KeyboadStates {
  STATE_ABC = 0,
  STATE_SPECIAL = 1,
};

HW_Controls::HW_Controls() {}

HW_Controls::~HW_Controls() {}

u8 HW_Controls::CheckPressedKey(s16 &T_X, s16 &T_Y, u8 &KeyboardState) {
  if (KeyboardState == STATE_SPECIAL)
    return CheckPressedKeySpecial(T_X, T_Y);
  else if (KeyboardState == STATE_ABC)  // Actually useless
    return CheckPressedKeyABC(T_X, T_Y);
  else
    return CheckPressedKeyABC(T_X, T_Y);
}

u8 HW_Controls::CheckPressedKeyABC(s16 &T_X, s16 &T_Y) {
  // 0 - 9
  if (T_Y < 41) {
    if (T_X < 33)
      return HBKB_KEY_1;
    else if (T_X > 32 && T_X < 65)
      return HBKB_KEY_2;
    else if (T_X > 64 && T_X < 97)
      return HBKB_KEY_3;
    else if (T_X > 96 && T_X < 129)
      return HBKB_KEY_4;
    else if (T_X > 128 && T_X < 161)
      return HBKB_KEY_5;
    else if (T_X > 160 && T_X < 193)
      return HBKB_KEY_6;
    else if (T_X > 192 && T_X < 225)
      return HBKB_KEY_7;
    else if (T_X > 224 && T_X < 257)
      return HBKB_KEY_8;
    else if (T_X > 256 && T_X < 289)
      return HBKB_KEY_9;
    else if (T_X > 288)
      return HBKB_KEY_0;
    else
      return HBKB_KEY_NONE;
  } else if (T_Y > 40 && T_Y < 81)  // QWERTYUIOP
  {
    if (T_X < 33)
      return HBKB_KEY_Q;
    else if (T_X > 32 && T_X < 65)
      return HBKB_KEY_W;
    else if (T_X > 64 && T_X < 97)
      return HBKB_KEY_E;
    else if (T_X > 96 && T_X < 129)
      return HBKB_KEY_R;
    else if (T_X > 128 && T_X < 161)
      return HBKB_KEY_T;
    else if (T_X > 160 && T_X < 193)
      return HBKB_KEY_Y;
    else if (T_X > 192 && T_X < 225)
      return HBKB_KEY_U;
    else if (T_X > 224 && T_X < 257)
      return HBKB_KEY_I;
    else if (T_X > 256 && T_X < 289)
      return HBKB_KEY_O;
    else if (T_X > 288)
      return HBKB_KEY_P;
    else
      return HBKB_KEY_NONE;
  } else if (T_Y > 80 && T_Y < 121)  // ASDFGHJKL
  {
    if (T_X < 33)
      return HBKB_KEY_A;
    else if (T_X > 32 && T_X < 65)
      return HBKB_KEY_S;
    else if (T_X > 64 && T_X < 97)
      return HBKB_KEY_D;
    else if (T_X > 96 && T_X < 129)
      return HBKB_KEY_F;
    else if (T_X > 128 && T_X < 161)
      return HBKB_KEY_G;
    else if (T_X > 160 && T_X < 193)
      return HBKB_KEY_H;
    else if (T_X > 192 && T_X < 225)
      return HBKB_KEY_J;
    else if (T_X > 224 && T_X < 257)
      return HBKB_KEY_K;
    else if (T_X > 256 && T_X < 289)
      return HBKB_KEY_L;
    else if (T_X > 288)
      return HBKB_KEY_BACK;  // Part of the Back Key is in tis Line
    else
      return HBKB_KEY_NONE;
  } else if (T_Y > 120 && T_Y < 161)  // ZXCVBNM
  {
    if (T_X < 65)
      return HBKB_KEY_CHANGEKEYS;  // Change to Special Characters
    else if (T_X > 64 && T_X < 97)
      return HBKB_KEY_Z;
    else if (T_X > 96 && T_X < 129)
      return HBKB_KEY_X;
    else if (T_X > 128 && T_X < 161)
      return HBKB_KEY_C;
    else if (T_X > 160 && T_X < 193)
      return HBKB_KEY_V;
    else if (T_X > 192 && T_X < 225)
      return HBKB_KEY_B;
    else if (T_X > 224 && T_X < 257)
      return HBKB_KEY_N;
    else if (T_X > 256 && T_X < 289)
      return HBKB_KEY_M;
    else if (T_X > 288)
      return HBKB_KEY_BACK;  // Second Part of Key Back
    else
      return HBKB_KEY_NONE;
  } else if (T_Y > 160 && T_Y < 201)  // Caps, Spacebar, Shift
  {
    if (T_X < 65)
      return HBKB_KEY_CAPS;
    else if (T_X > 64 && T_X < 257)
      return HBKB_KEY_SPACE;
    else if (T_X > 256)
      return HBKB_KEY_SHIFT;
    else
      return HBKB_KEY_NONE;
  } else if (T_Y > 200)  // OK / Cancel
  {
    if (T_X < 161)
      return HBKB_KEY_CANCEL;
    else
      return HBKB_KEY_ENTER;
  } else
    return HBKB_KEY_NONE;
}

u8 HW_Controls::CheckPressedKeySpecial(s16 &T_X, s16 &T_Y) {
  // 0 - 9
  if (T_Y < 41) {
    if (T_X < 33)
      return HBKB_KEY_1;
    else if (T_X > 32 && T_X < 65)
      return HBKB_KEY_2;
    else if (T_X > 64 && T_X < 97)
      return HBKB_KEY_3;
    else if (T_X > 96 && T_X < 129)
      return HBKB_KEY_4;
    else if (T_X > 128 && T_X < 161)
      return HBKB_KEY_5;
    else if (T_X > 160 && T_X < 193)
      return HBKB_KEY_6;
    else if (T_X > 192 && T_X < 225)
      return HBKB_KEY_7;
    else if (T_X > 224 && T_X < 257)
      return HBKB_KEY_8;
    else if (T_X > 256 && T_X < 289)
      return HBKB_KEY_9;
    else if (T_X > 288)
      return HBKB_KEY_0;
    else
      return HBKB_KEY_NONE;
  } else if (T_Y > 40 && T_Y < 81)  // !"§$%&/()=
  {
    if (T_X < 33)
      return HBKB_KEY_SPECIAL_A;
    else if (T_X > 32 && T_X < 65)
      return HBKB_KEY_SPECIAL_B;
    else if (T_X > 64 && T_X < 97)
      return HBKB_KEY_SPECIAL_C;
    else if (T_X > 96 && T_X < 129)
      return HBKB_KEY_SPECIAL_D;
    else if (T_X > 128 && T_X < 161)
      return HBKB_KEY_SPECIAL_E;
    else if (T_X > 160 && T_X < 193)
      return HBKB_KEY_SPECIAL_F;
    else if (T_X > 192 && T_X < 225)
      return HBKB_KEY_SPECIAL_G;
    else if (T_X > 224 && T_X < 257)
      return HBKB_KEY_SPECIAL_H;
    else if (T_X > 256 && T_X < 289)
      return HBKB_KEY_SPECIAL_I;
    else if (T_X > 288)
      return HBKB_KEY_SPECIAL_J;
    else
      return HBKB_KEY_NONE;
  } else if (T_Y > 80 && T_Y < 121)  // ASDFGHJKL
  {
    if (T_X < 33)
      return HBKB_KEY_SPECIAL_K;
    else if (T_X > 32 && T_X < 65)
      return HBKB_KEY_SPECIAL_L;
    else if (T_X > 64 && T_X < 97)
      return HBKB_KEY_SPECIAL_M;
    else if (T_X > 96 && T_X < 129)
      return HBKB_KEY_SPECIAL_N;
    else if (T_X > 128 && T_X < 161)
      return HBKB_KEY_SPECIAL_O;
    else if (T_X > 160 && T_X < 193)
      return HBKB_KEY_SPECIAL_P;
    else if (T_X > 192 && T_X < 225)
      return HBKB_KEY_SPECIAL_Q;
    else if (T_X > 224 && T_X < 257)
      return HBKB_KEY_SPECIAL_R;
    else if (T_X > 256 && T_X < 289)
      return HBKB_KEY_SPECIAL_S;
    else if (T_X > 288)
      return HBKB_KEY_BACK;  // Part of the Back Key is in tis Line
    else
      return HBKB_KEY_NONE;
  } else if (T_Y > 120 && T_Y < 161)  // ZXCVBNM
  {
    if (T_X < 65)
      return HBKB_KEY_CHANGEKEYS;  // Change to ABC Characters
    else if (T_X > 64 && T_X < 97)
      return HBKB_KEY_SPECIAL_T;
    else if (T_X > 96 && T_X < 129)
      return HBKB_KEY_SPECIAL_U;
    else if (T_X > 128 && T_X < 161)
      return HBKB_KEY_SPECIAL_V;
    else if (T_X > 160 && T_X < 193)
      return HBKB_KEY_SPECIAL_W;
    else if (T_X > 192 && T_X < 225)
      return HBKB_KEY_SPECIAL_X;
    else if (T_X > 224 && T_X < 257)
      return HBKB_KEY_SPECIAL_Y;
    else if (T_X > 256 && T_X < 289)
      return HBKB_KEY_SPECIAL_Z;
    else if (T_X > 288)
      return HBKB_KEY_BACK;  // Second Part of Key Back
    else
      return HBKB_KEY_NONE;
  } else if (T_Y > 160 && T_Y < 201)  // Caps, Spacebar, Shift
  {
    if (T_X < 65)
      return HBKB_KEY_CAPS;
    else if (T_X > 64 && T_X < 257)
      return HBKB_KEY_SPACE;
    else if (T_X > 256)
      return HBKB_KEY_SHIFT;
    else
      return HBKB_KEY_NONE;
  } else if (T_Y > 200)  // OK / Cancel
  {
    if (T_X < 161)
      return HBKB_KEY_CANCEL;
    else
      return HBKB_KEY_ENTER;
  } else
    return HBKB_KEY_NONE;
}