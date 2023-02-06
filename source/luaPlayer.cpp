/*
 *   This file is part of lpp-3ds-next
 *   based on https://github.com/Rinnegatamante/lpp-3ds/
 *   Copyright (C) 2021-2023 Tobi-D7
 */

#include <ErrorHelper.hpp>
#include <cstring>
#include <iostream>
#include <luaplayer.hpp>
#include <memory>

void Npi_Error(std::string Error) {
  std::cout << "Error:\n" << Error << std::endl;
  ErrorHelper::WriteErr(Error);
  exit(-1);
}

static void InitLibraries(lua_State *LState) {
  luaL_openlibs(LState); // Standard Libraries.
  luaTimer_init(LState);
  luaNetwork_init(LState);
  luaCore_init(LState);
  luaControls_init(LState);
  luaSystem_init(LState);
  luaScreen_init(LState);
  luaGraphics_init(LState);
  luaRender_init(LState);
  luaKeyboard_init(LState);
  luaMic_init(LState);
  luaCamera_init(LState);
}

void Run(std::string path) {
  if (path == "")
    return;

  std::pair<int, std::string> Status = std::make_pair(0, "");
  lua_State *LUAScript = luaL_newstate();
  InitLibraries(LUAScript);

  Status.first = luaL_loadfile(LUAScript, path.c_str());
  if (Status.first == 0)
    Status.first = lua_pcall(LUAScript, 0, LUA_MULTRET, 0);

  if (Status.first) {                            // 1+, an error occured.
    Status.second = lua_tostring(LUAScript, -1); // Return error message.
    lua_pop(LUAScript, 1); // Remove error message from LUA Script.
  };

  lua_close(LUAScript);

  if (Status.first) {
    if (strstr(Status.second.c_str(), "lpp_exit_04"))
      exit(0);
    Npi_Error(Status.second);
  };
}