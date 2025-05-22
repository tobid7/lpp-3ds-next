/*
 *   This file is part of lpp-3ds-next
 *   based on https://github.com/Rinnegatamante/lpp-3ds/
 *   Copyright (C) 2021-2025 tobid7
 */

#include <3ds.h>

#include <ErrorHelper.hpp>
#include <cstring>
#include <iostream>
#include <luaplayer.hpp>
#include <memory>
#include <sstream>

void Npi_Error(std::string Error) {
  std::cout << "Error:\n" << Error << std::endl;
  ErrorHelper::WriteErr(Error);
  errorConf* cfg = new errorConf;
  errorInit(cfg, errorType::ERROR_TEXT, CFG_LANGUAGE_EN);
  errorText(cfg, std::string("Lua Error:\n" + Error).c_str());
  errorDisp(cfg);
  exit(-1);
}

void CxxError(const char* msg) {
  std::string err_text = std::string("C++ Exception:\n") + msg;
  std::cout << err_text << std::endl;
  ErrorHelper::WriteErr(err_text);
  errorConf* cfg = new errorConf;
  errorInit(cfg, errorType::ERROR_TEXT, CFG_LANGUAGE_EN);
  errorText(cfg, std::string(err_text).c_str());
  errorDisp(cfg);
  exit(-1);
}

void CxxExceptionHandler() {
  std::exception_ptr e_ = std::current_exception();
  if (e_) {
    try {
      std::rethrow_exception(e_);
    } catch (const std::exception& e) {
      CxxError(e.what());
    } catch (...) {
      CxxError("Unknown Exception");
    }
  }
  std::abort();
}

static void InitLibraries(lua_State* LState) {
  luaL_openlibs(LState);  // Standard Libraries.
  luaTimer_init(LState);
  luaNetwork_init(LState);
  luaCore_init(LState);
  luaControls_init(LState);
  luaSystem_init(LState);
  luaSound_init(LState);
  luaScreen_init(LState);
  luaGraphics_init(LState);
  luaRender_init(LState);
  luaKeyboard_init(LState);
  luaMic_init(LState);
  luaCamera_init(LState);
  luaVideo_init(LState);
}

std::string detail_error(lua_State* LState) {
  std::stringstream ss;
  const char* errorMsg = lua_tostring(LState, -1);
  ss << "Lua error: " << errorMsg << std::endl;
  // Read more Info
  // Crashes for 3ds idk why
  // lua_Debug debugInfo;
  // lua_getstack(LState, 1, &debugInfo);
  // lua_getinfo(LState, "l", &debugInfo);
  // int lineNumber = debugInfo.currentline;
  // ss << "Error at line: " << lineNumber << std::endl;
  // Create stacktrace
  lua_Debug entry;
  int depth = 0;
  while (lua_getstack(LState, depth, &entry)) {
    lua_getinfo(LState, "nSl", &entry);
    const char* functionName = entry.name ? entry.name : "?";
    const char* sourceName = entry.short_src ? entry.short_src : "?";
    int line = entry.currentline;
    ss << "  [" << depth << "] " << functionName << " in " << sourceName << ":"
       << line << std::endl;
    depth++;
  }
  return ss.str();
}

void Run(std::string path) {
  if (path == "") return;
  std::set_terminate(CxxExceptionHandler);

  std::pair<int, std::string> Status = std::make_pair(0, "");
  lua_State* LUAScript = luaL_newstate();
  InitLibraries(LUAScript);

  Status.first = luaL_loadfile(LUAScript, path.c_str());
  if (Status.first == 0) Status.first = lua_pcall(LUAScript, 0, LUA_MULTRET, 0);

  if (Status.first) {                         // 1+, an error occured.
    Status.second = detail_error(LUAScript);  // Return error message.
    lua_pop(LUAScript, 1);  // Remove error message from LUA Script.
  };

  lua_close(LUAScript);

  if (Status.first) {
    // Fake Error or so idk (made by Rinnegatamante)
    if (strstr(Status.second.c_str(), "lpp_exit_04")) exit(0);
    Npi_Error(Status.second);
  };
}