/*
*   This file is part of lpp-3ds-next
*   based on https://github.com/Rinnegatamante/lpp-3ds/
*   Copyright (C) 2021-2023 Tobi-D7
*/

#pragma once
#include <stdlib.h>
#include <lua/lua.hpp>
#include <string>

extern void luaC_collectgarbage (lua_State *L);

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(val, min, max) ((val)>(max)?(max):((val)<(min)?(min):(val)))

void Run(std::string path);
void luaC_collectgarbage (lua_State *L);

//void luaScreen_init(lua_State *L);
//void luaControls_init(lua_State *L);
//void luaSystem_init(lua_State *L);
void luaTimer_init(lua_State *L);
//void luaSound_init(lua_State *L);
//void luaVideo_init(lua_State *L);
//void luaGraphics_init(lua_State *L);
//void luaNetwork_init(lua_State *L);
//void luaCamera_init(lua_State *L);
//void luaRender_init(lua_State *L);
//void luaMic_init(lua_State *L);
//void luaCore_init(lua_State *L);
//void luaKeyboard_init(lua_State *L);
//void stackDump (lua_State *L);