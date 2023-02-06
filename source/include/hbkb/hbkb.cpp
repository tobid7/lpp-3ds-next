/*
 * hbkb.cpp
 *
 *  Created on: Sep 12, 2015
 *      Author: jbr373
 *
 *      #############
 *      # GFX CODE BY SMEALUM
 *      # http://www.github.com/smealum/
 *      #############
 *
 *      HBKB (HomeBrewKeyBoard) General Information:
 *
 *      HBKB is intended to be a useful Keyboard for Homebrew
 *      Applications.
 *
 */

#include <3ds.h>
#include <string>

#include "hbkb.h"
#include "hw_controls.h"
#include "hw_graphics.h"

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
  HBKB_KEY_SPECIAL_A = 66, // !
  HBKB_KEY_SPECIAL_B = 67, // "
  HBKB_KEY_SPECIAL_C = 68, // §
  HBKB_KEY_SPECIAL_D = 69, // $
  HBKB_KEY_SPECIAL_E = 70, // %
  HBKB_KEY_SPECIAL_F = 71, // &
  HBKB_KEY_SPECIAL_G = 72, // /
  HBKB_KEY_SPECIAL_H = 73, // (
  HBKB_KEY_SPECIAL_I = 74, // )
  HBKB_KEY_SPECIAL_J = 75, // ?

  // Special - Row 1
  HBKB_KEY_SPECIAL_K = 76, // *
  HBKB_KEY_SPECIAL_L = 77, // '
  HBKB_KEY_SPECIAL_M = 78, // <
  HBKB_KEY_SPECIAL_N = 79, // >
  HBKB_KEY_SPECIAL_O = 80, // +
  HBKB_KEY_SPECIAL_P = 81, // [
  HBKB_KEY_SPECIAL_Q = 82, // ]
  HBKB_KEY_SPECIAL_R = 83, // {
  HBKB_KEY_SPECIAL_S = 84, // }

  // Special - Row 2
  HBKB_KEY_SPECIAL_T = 85, // ,
  HBKB_KEY_SPECIAL_U = 86, // ;
  HBKB_KEY_SPECIAL_V = 87, // .
  HBKB_KEY_SPECIAL_W = 88, // :
  HBKB_KEY_SPECIAL_X = 89, // -
  HBKB_KEY_SPECIAL_Y = 90, // _
  HBKB_KEY_SPECIAL_Z = 99, // =

  HBKB_KEY_ENTER = 91,
  HBKB_KEY_BACK = 92,
  HBKB_KEY_SHIFT = 93,
  HBKB_KEY_CAPS = 94,
  HBKB_KEY_CHANGEKEYS = 95,
  HBKB_KEY_CANCEL = 96
};

enum ReturnValue {
  RETURN_UNKNOWN = 0,
  RETURN_FINISHED = 1,
  RETURN_ENTEREDKEY = 2,
  RETURN_CANCEL = 3,
  RETURN_NONE = 4,
};

enum KeyboadStates {
  STATE_ABC = 0,
  STATE_SPECIAL = 1,
};

HB_Keyboard::HB_Keyboard() {
  UserInput = "";
  KeyboardState = STATE_ABC;
  CurrentKey = HBKB_KEY_NONE;
  isShift = true;
  isCaps = false;
}

HB_Keyboard::~HB_Keyboard() {}

// Other Classes (Stack)
HW_Graphics sGraphics;
HW_Controls sControls;

void HB_Keyboard::KeyInteraction(u8 &Key) {
  // Change Shift State
  if (Key == HBKB_KEY_SHIFT) // Shift Management
  {
    if (isShift)
      isShift = false;
    else
      isShift = true;
  } else if (Key == HBKB_KEY_CAPS) // Caps Management
  {
    if (isCaps)
      isCaps = false;
    else
      isCaps = true;
  } else if (Key == HBKB_KEY_CHANGEKEYS) // Keyboard Change
  {
    if (KeyboardState == 0)
      KeyboardState = STATE_SPECIAL; // To Special
    else
      KeyboardState = STATE_ABC; // To ABC
  } else if (Key != HBKB_KEY_ENTER &&
             Key != HBKB_KEY_CANCEL) // Not Enter or Cancel
    ChangeString(Key);               // Back Key is handled here.
  else
    ChangeString(Key); // Back Key is still handled here.

  if (isShift && Key != HBKB_KEY_NONE &&
      Key !=
          HBKB_KEY_SHIFT) // Change Shift if normal Key was Pressed with Shift
    isShift = false;
}

void HB_Keyboard::ChangeString(u8 &Key) {
  if (Key == HBKB_KEY_BACK) {
    // Remove 1 Char from String
    if (UserInput != "")
      UserInput.erase(UserInput.end() - 1, UserInput.end());
  } else {
    if (Key == HBKB_KEY_A) {
      if (isShift || isCaps)
        UserInput += "A";
      else
        UserInput += "a";
    } else if (Key == HBKB_KEY_B) {
      if (isShift || isCaps)
        UserInput += "B";
      else
        UserInput += "b";
    } else if (Key == HBKB_KEY_C) {
      if (isShift || isCaps)
        UserInput += "C";
      else
        UserInput += "c";
    } else if (Key == HBKB_KEY_D) {
      if (isShift || isCaps)
        UserInput += "D";
      else
        UserInput += "d";
    } else if (Key == HBKB_KEY_E) {
      if (isShift || isCaps)
        UserInput += "E";
      else
        UserInput += "e";
    } else if (Key == HBKB_KEY_F) {
      if (isShift || isCaps)
        UserInput += "F";
      else
        UserInput += "f";
    } else if (Key == HBKB_KEY_G) {
      if (isShift || isCaps)
        UserInput += "G";
      else
        UserInput += "g";
    } else if (Key == HBKB_KEY_H) {
      if (isShift || isCaps)
        UserInput += "H";
      else
        UserInput += "h";
    } else if (Key == HBKB_KEY_I) {
      if (isShift || isCaps)
        UserInput += "I";
      else
        UserInput += "i";
    } else if (Key == HBKB_KEY_J) {
      if (isShift || isCaps)
        UserInput += "J";
      else
        UserInput += "j";
    } else if (Key == HBKB_KEY_K) {
      if (isShift || isCaps)
        UserInput += "K";
      else
        UserInput += "k";
    } else if (Key == HBKB_KEY_L) {
      if (isShift || isCaps)
        UserInput += "L";
      else
        UserInput += "l";
    } else if (Key == HBKB_KEY_M) {
      if (isShift || isCaps)
        UserInput += "M";
      else
        UserInput += "m";
    } else if (Key == HBKB_KEY_N) {
      if (isShift || isCaps)
        UserInput += "N";
      else
        UserInput += "n";
    } else if (Key == HBKB_KEY_O) {
      if (isShift || isCaps)
        UserInput += "O";
      else
        UserInput += "o";
    } else if (Key == HBKB_KEY_P) {
      if (isShift || isCaps)
        UserInput += "P";
      else
        UserInput += "p";
    } else if (Key == HBKB_KEY_Q) {
      if (isShift || isCaps)
        UserInput += "Q";
      else
        UserInput += "q";
    } else if (Key == HBKB_KEY_R) {
      if (isShift || isCaps)
        UserInput += "R";
      else
        UserInput += "r";
    } else if (Key == HBKB_KEY_S) {
      if (isShift || isCaps)
        UserInput += "S";
      else
        UserInput += "s";
    } else if (Key == HBKB_KEY_T) {
      if (isShift || isCaps)
        UserInput += "T";
      else
        UserInput += "t";
    } else if (Key == HBKB_KEY_U) {
      if (isShift || isCaps)
        UserInput += "U";
      else
        UserInput += "u";
    } else if (Key == HBKB_KEY_V) {
      if (isShift || isCaps)
        UserInput += "V";
      else
        UserInput += "v";
    } else if (Key == HBKB_KEY_W) {
      if (isShift || isCaps)
        UserInput += "W";
      else
        UserInput += "w";
    } else if (Key == HBKB_KEY_X) {
      if (isShift || isCaps)
        UserInput += "X";
      else
        UserInput += "x";
    } else if (Key == HBKB_KEY_Y) {
      if (isShift || isCaps)
        UserInput += "Y";
      else
        UserInput += "y";
    } else if (Key == HBKB_KEY_Z) {
      if (isShift || isCaps)
        UserInput += "Z";
      else
        UserInput += "z";
    } else if (Key == HBKB_KEY_0)
      UserInput += "0";
    else if (Key == HBKB_KEY_1)
      UserInput += "1";
    else if (Key == HBKB_KEY_2)
      UserInput += "2";
    else if (Key == HBKB_KEY_3)
      UserInput += "3";
    else if (Key == HBKB_KEY_4)
      UserInput += "4";
    else if (Key == HBKB_KEY_5)
      UserInput += "5";
    else if (Key == HBKB_KEY_6)
      UserInput += "6";
    else if (Key == HBKB_KEY_7)
      UserInput += "7";
    else if (Key == HBKB_KEY_8)
      UserInput += "8";
    else if (Key == HBKB_KEY_9)
      UserInput += "9";
    else if (Key == HBKB_KEY_SPECIAL_A)
      UserInput += "!";
    else if (Key == HBKB_KEY_SPECIAL_B)
      UserInput += "\"";
    else if (Key == HBKB_KEY_SPECIAL_C)
      UserInput += "§";
    else if (Key == HBKB_KEY_SPECIAL_D)
      UserInput += "$";
    else if (Key == HBKB_KEY_SPECIAL_E)
      UserInput += "%";
    else if (Key == HBKB_KEY_SPECIAL_F)
      UserInput += "&";
    else if (Key == HBKB_KEY_SPECIAL_G)
      UserInput += "/";
    else if (Key == HBKB_KEY_SPECIAL_H)
      UserInput += "(";
    else if (Key == HBKB_KEY_SPECIAL_I)
      UserInput += ")";
    else if (Key == HBKB_KEY_SPECIAL_J)
      UserInput += "?";
    else if (Key == HBKB_KEY_SPECIAL_K)
      UserInput += "*";
    else if (Key == HBKB_KEY_SPECIAL_L)
      UserInput += "'";
    else if (Key == HBKB_KEY_SPECIAL_M)
      UserInput += "<";
    else if (Key == HBKB_KEY_SPECIAL_N)
      UserInput += ">";
    else if (Key == HBKB_KEY_SPECIAL_O)
      UserInput += "+";
    else if (Key == HBKB_KEY_SPECIAL_P)
      UserInput += "[";
    else if (Key == HBKB_KEY_SPECIAL_Q)
      UserInput += "]";
    else if (Key == HBKB_KEY_SPECIAL_R)
      UserInput += "{";
    else if (Key == HBKB_KEY_SPECIAL_S)
      UserInput += "}";
    else if (Key == HBKB_KEY_SPECIAL_T)
      UserInput += ",";
    else if (Key == HBKB_KEY_SPECIAL_U)
      UserInput += ";";
    else if (Key == HBKB_KEY_SPECIAL_V)
      UserInput += ".";
    else if (Key == HBKB_KEY_SPECIAL_W)
      UserInput += ":";
    else if (Key == HBKB_KEY_SPECIAL_X)
      UserInput += "-";
    else if (Key == HBKB_KEY_SPECIAL_Y)
      UserInput += "_";
    else if (Key == HBKB_KEY_SPECIAL_Z)
      UserInput += "=";
    else if (Key == HBKB_KEY_SPACE)
      UserInput += " ";
    else {
    }
  }
}

u8 HB_Keyboard::HBKB_CallKeyboard(touchPosition TouchScreenPos) {
  // Key Check
  static u8 OldKeyPress = 0;
  static bool SpamKey = false;

  // Touch X & Y
  s16 T_X = TouchScreenPos.px;
  s16 T_Y = TouchScreenPos.py;

  // Key Presses
  // Ignore 0 | 0 Coordinates (No Touch Screen)
  if (T_X == 0 && T_Y == 0) {
    OldKeyPress = 0;
    CurrentKey = 0;
  } else {
    // Check Pressed Key
    CurrentKey = sControls.CheckPressedKey(T_X, T_Y, KeyboardState);

    // Key Interaction
    if (CurrentKey == OldKeyPress) // Skip Input Check if same Key
    {
      if (SpamKey) // Do we print the same Key for every Frame?
        KeyInteraction(CurrentKey);
    } else // Different Key, so add Key Instantly
    {
      OldKeyPress = CurrentKey;
      KeyInteraction(CurrentKey);
    }
  }

  // Draw Keyboard dependend on state
  sGraphics.DrawKeyboard(KeyboardState, T_X, T_Y, isShift, isCaps, CurrentKey);

  // Enter, Cancel and Input Return

  // User used Enter and ended the input
  if (CurrentKey == HBKB_KEY_ENTER) {
    CurrentKey = 0;
    return RETURN_FINISHED;
  } else if (CurrentKey != 0 && CurrentKey != HBKB_KEY_CANCEL &&
             CurrentKey !=
                 HBKB_KEY_ENTER) // User entered any Key except Cancel and Enter
  {
    CurrentKey = 0;
    return RETURN_ENTEREDKEY;
  } else if (CurrentKey == HBKB_KEY_CANCEL) // User Canceled Keyboard Input
  {
    CurrentKey = 0;
    return RETURN_CANCEL;
  } else if (CurrentKey == 0) // No Key Pressed
  {
    CurrentKey = 0;
    return RETURN_NONE;
  } else // Unknown Key Value or Key Value not set
  {
    CurrentKey = 0;
    return RETURN_UNKNOWN;
  }
}

std::string HB_Keyboard::HBKB_CheckKeyboardInput() {
  // Return Input
  return UserInput;
}

void HB_Keyboard::HBKB_Clean() {
  KeyboardState = STATE_ABC;
  UserInput = "";
  isCaps = false;
  isShift = true;
}