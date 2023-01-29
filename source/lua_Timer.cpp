/*
*   This file is part of lpp-3ds-next
*   based on https://github.com/Rinnegatamante/lpp-3ds/
*   Copyright (C) 2021-2023 Tobi-D7
*/

#include <luaplayer.hpp>

#include <3ds.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <utils.h>

static int lua_newT(lua_State *L) {
  int argc = lua_gettop(L);
  if (argc != 0)
    return luaL_error(L, "wrong number of arguments.");
  Timer *new_timer = (Timer *)malloc(sizeof(Timer));
  new_timer->tick = osGetTime();
  new_timer->magic = 0x4C544D52;
  new_timer->isPlaying = true;
  lua_pushinteger(L, (u32)new_timer);
  return 1;
}

static int lua_time(lua_State *L) {
  int argc = lua_gettop(L);
  if (argc != 1)
    return luaL_error(L, "wrong number of arguments.");
  Timer *src = (Timer *)luaL_checkinteger(L, 1);
  if (src->magic != 0x4C544D52)
    return luaL_error(L, "attempt to access wrong memory block type");
  if (src->isPlaying) {
    lua_pushinteger(L, (osGetTime() - src->tick));
  } else {
    lua_pushinteger(L, src->tick);
  }
  return 1;
}

static int lua_pause(lua_State *L) {
  int argc = lua_gettop(L);
  if (argc != 1)
    return luaL_error(L, "wrong number of arguments.");
  Timer *src = (Timer *)luaL_checkinteger(L, 1);
  if (src->magic != 0x4C544D52)
    return luaL_error(L, "attempt to access wrong memory block type");
  if (src->isPlaying) {
    src->isPlaying = false;
    src->tick = (osGetTime() - src->tick);
  }
  return 0;
}

static int lua_resume(lua_State *L) {
  int argc = lua_gettop(L);
  if (argc != 1)
    return luaL_error(L, "wrong number of arguments.");
  Timer *src = (Timer *)luaL_checkinteger(L, 1);
  if (src->magic != 0x4C544D52)
    return luaL_error(L, "attempt to access wrong memory block type");
  if (!src->isPlaying) {
    src->isPlaying = true;
    src->tick = (osGetTime() - src->tick);
  }
  return 0;
}

static int lua_reset(lua_State *L) {
  int argc = lua_gettop(L);
  if (argc != 1)
    return luaL_error(L, "wrong number of arguments.");
  Timer *src = (Timer *)luaL_checkinteger(L, 1);
  if (src->magic != 0x4C544D52)
    return luaL_error(L, "attempt to access wrong memory block type");
  if (src->isPlaying)
    src->tick = osGetTime();
  else
    src->tick = 0;
  return 0;
}

static int lua_set(lua_State *L) {
  int argc = lua_gettop(L);
  if (argc != 2)
    return luaL_error(L, "wrong number of arguments.");
  Timer *src = (Timer *)luaL_checkinteger(L, 1);
  uint32_t val = (uint32_t)luaL_checkinteger(L, 2);
  if (src->magic != 0x4C544D52)
    return luaL_error(L, "attempt to access wrong memory block type");
  if (src->isPlaying)
    src->tick = osGetTime() + val;
  else
    src->tick = val;
  return 0;
}

static int lua_wisPlaying(lua_State *L) {
  int argc = lua_gettop(L);
  if (argc != 1)
    return luaL_error(L, "wrong number of arguments.");
  Timer *src = (Timer *)luaL_checkinteger(L, 1);
  if (src->magic != 0x4C544D52)
    return luaL_error(L, "attempt to access wrong memory block type");
  lua_pushboolean(L, src->isPlaying);
  return 1;
}

static int lua_destroy(lua_State *L) {
  int argc = lua_gettop(L);
  if (argc != 1)
    return luaL_error(L, "wrong number of arguments.");
  Timer *timer = (Timer *)luaL_checkinteger(L, 1);
  if (timer->magic != 0x4C544D52)
    return luaL_error(L, "attempt to access wrong memory block type");
  free(timer);
  return 1;
}

// Register our Timer Functions
static const luaL_Reg Timer_functions[] = {{"new", lua_newT},
                                           {"getTime", lua_time},
                                           {"setTime", lua_set},
                                           {"destroy", lua_destroy},
                                           {"pause", lua_pause},
                                           {"resume", lua_resume},
                                           {"reset", lua_reset},
                                           {"isPlaying", lua_wisPlaying},
                                           {0, 0}};

void luaTimer_init(lua_State *L) {
  lua_newtable(L);
  luaL_setfuncs(L, Timer_functions, 0);
  lua_setglobal(L, "Timer");
}