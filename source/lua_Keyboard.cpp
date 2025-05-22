/*
 *   This file is part of lpp-3ds-next
 *   based on https://github.com/Rinnegatamante/lpp-3ds/
 *   Copyright (C) 2021-2025 tobid7
 */

#include <3ds.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "Graphics.hpp"
#include "hbkb.h"
#include "luaplayer.hpp"

#define stringify(str) #str
#define VariableRegister(lua, value)      \
  do {                                    \
    lua_pushinteger(lua, value);          \
    lua_setglobal(lua, stringify(value)); \
  } while (0)
HB_Keyboard keyboard;
u8 keystate = 0;

static int lua_setText(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1) return luaL_error(L, "wrong number of arguments");
#endif
  const char *text = luaL_checkstring(L, 1);
  std::string s(text);
  keyboard.UserInput = s;
  return 0;
}

static int lua_show(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 0) return luaL_error(L, "wrong number of arguments");
#endif
  touchPosition touch;
  hidScanInput();
  hidTouchRead(&touch);
  keystate = keyboard.HBKB_CallKeyboard(touch);
  return 0;
}

static int lua_state(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 0) return luaL_error(L, "wrong number of arguments");
#endif
  lua_pushinteger(L, keystate);
  return 1;
}

static int lua_input(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 0) return luaL_error(L, "wrong number of arguments");
#endif
  if (keystate == 3) keyboard.HBKB_Clean();
  lua_pushstring(L, keyboard.HBKB_CheckKeyboardInput().c_str());
  return 1;
}

static int lua_clear(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 0) return luaL_error(L, "wrong number of arguments");
#endif
  keyboard.HBKB_Clean();
  keystate = 0;
  return 0;
}

// Register our Keyboard Functions
static const luaL_Reg Keyboard_functions[] = {
    {"setText", lua_setText}, {"show", lua_show},   {"getState", lua_state},
    {"getInput", lua_input},  {"clear", lua_clear}, {0, 0}};

void luaKeyboard_init(lua_State *L) {
  lua_newtable(L);
  luaL_setfuncs(L, Keyboard_functions, 0);
  lua_setglobal(L, "Keyboard");
  u8 FINISHED = 1;
  u8 PRESSED = 2;
  u8 CLEANED = 3;
  u8 NOT_PRESSED = 4;
  VariableRegister(L, FINISHED);
  VariableRegister(L, PRESSED);
  VariableRegister(L, CLEANED);
  VariableRegister(L, NOT_PRESSED);
}