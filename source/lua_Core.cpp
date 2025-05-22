/*
 *   This file is part of lpp-3ds-next
 *   based on https://github.com/Rinnegatamante/lpp-3ds/
 *   Copyright (C) 2021-2025 tobid7
 */

#include <3ds.h>
#include <utils.h>

#include <audio.hpp>
#include <cstring>
#include <luaplayer.hpp>

// Different kind of syscalls
typedef Result (*func_1x)();
typedef Result (*func_2x_type1)(u32 arg);
typedef Result (*func_3x_type1)(u8 arg, u32 arg2);
typedef Result (*func_3x_type2)(u8 arg, u64 arg2);
typedef Result (*func_9x_type1)(int arg, u32 arg2, u32 arg3, float arg4,
                                float arg5, void *arg6, void *arg7, u32 arg8);

// Different kind of databases for parser optimization
typedef struct {
  const char *name;
  func_1x callback;
} func_1x_db;

typedef struct {
  const char *name;
  func_2x_type1 callback;
} func_2x_db;

typedef struct {
  const char *name;
  func_3x_type1 callback_type1;  // argsize 5 (1+4)
  func_3x_type2 callback_type2;  // argsize 9 (1+4(pointer)) since Lua doesn't
                                 // support 64bit integers natively
} func_3x_db;
typedef struct {
  const char *name;
  func_9x_type1 callback_type1;  // csndPlaysound-like calls
} func_9x_db;

Result exit_am() {
  amExit();
  return 0;
}

Result exit_csnd() {
  csndExit();
  return 0;
}
Result exit_ndsp() {
  ndspExit();
  return 0;
}

// No args syscalls
func_1x_db db_1x[] = {
    {"amInit", amInit},     {"amExit", exit_am},     {"csndInit", csndInit},
    {"ndspInit", ndspInit}, {"csndExit", exit_csnd}, {"ndspExit", exit_ndsp},
};

// One arg syscall
func_2x_db db_2x[] = {
    // args size = 4
    {"AM_CancelCIAInstall", (func_2x_type1)AM_CancelCIAInstall},  // 0
    {"AM_FinishCiaInstall", (func_2x_type1)AM_FinishCiaInstall},  // 1
};

// Two args syscalls
func_3x_db db_3x[] = {
    // type 1
    {"AM_StartCiaInstall", (func_3x_type1)AM_StartCiaInstall},  // 0
    {"AM_GetTitleCount", (func_3x_type1)AM_GetTitleCount},      // 1
    // type 2
    {"AM_DeleteTitle", NULL, (func_3x_type2)AM_DeleteTitle},        // 2
    {"AM_DeleteAppTitle", NULL, (func_3x_type2)AM_DeleteAppTitle},  // 3
};

// Two args syscalls
func_9x_db db_9x[] = {
    // type 1
    {"csndPlaysound", (func_9x_type1)csndPlaySound},  // 0
};

static int lua_service(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1) return luaL_error(L, "wrong number of arguments.");
#endif
  const char *srv = luaL_checkstring(L, 1);
  Handle tmp;
  srvGetServiceHandle(&tmp, srv);
  lua_pushboolean(L, tmp);
  svcCloseHandle(tmp);

  // We have to manually restart a service if it's a default one of lpp-3ds
  const char *handle_list[7] = {"ptm:u",  "am",  "cfg:u", "hid",
                                "ir:rst", "apt", "ac"};
  func_1x restartService[7] = {ptmuInit,  amInit,  cfguInit, hidInit,
                               irrstInit, aptInit, acInit

  };
  for (int i = 0; i < 7; i++) {
    if (strstr(srv, handle_list[i]) != NULL) (restartService[i])();
  }

  return 1;
}

static int lua_execall(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc == 0) return luaL_error(L, "wrong number of arguments.");
#endif
  const char *call = luaL_checkstring(L, 1);

  // Parsing known syscalls
  if (argc == 1) {
    u32 known_syscalls = sizeof(db_1x) / sizeof(func_1x_db);
    for (int i = 0; i < (int)known_syscalls; i++) {
      if (strcmp(db_1x[i].name, call) == 0) {
        lua_pushinteger(L, db_1x[i].callback());
        return 1;
      }
    }
  } else if (argc == 2) {
    u32 known_syscalls = sizeof(db_2x) / sizeof(func_2x_db);
    for (int i = 0; i < (int)known_syscalls; i++) {
      if (strcmp(db_2x[i].name, call) == 0) {
        lua_pushinteger(L, db_2x[i].callback((u32)luaL_checkbuffer(L, 1)));
        return 1;
      }
    }
  } else if (argc == 3) {
    u32 known_syscalls = sizeof(db_3x) / sizeof(func_3x_db);
    for (int i = 0; i < (int)known_syscalls; i++) {
      if (strcmp(db_3x[i].name, call) == 0) {
        // Parsing arguments according to syscall
        if (i <= 1) {
          u8 arg1 = luaL_checkinteger(L, 2);
          u32 arg2 = luaL_checkinteger(L, 3);
          lua_pushinteger(L, db_3x[i].callback_type1(arg1, arg2));
        } else {
          u8 arg1 = luaL_checkinteger(L, 2);
          u64 *arg2 = (u64 *)luaL_checkinteger(L, 3);
          lua_pushinteger(L, db_3x[i].callback_type2(arg1, *arg2));
        }
        return 1;
      }
    }
  } else if (argc == 8) {
    u32 known_syscalls = sizeof(db_3x) / sizeof(func_3x_db);
    for (int i = 0; i < (int)known_syscalls; i++) {
      if (strcmp(db_3x[i].name, call) == 0) {
        int arg1 = luaL_checkinteger(L, 1);
        u32 arg2 = (u32)luaL_checkinteger(L, 2);
        u32 arg3 = (u32)luaL_checkinteger(L, 3);
        float arg4 = luaL_checknumber(L, 4);
        float arg5 = luaL_checknumber(L, 5);
        void *arg6 = luaL_checkbuffer(L, 6);
        void *arg7 = luaL_checkbuffer(L, 7);
        u32 arg8 = (u32)luaL_checkinteger(L, 8);
        // Parsing arguments according to syscall
        if (i <= 0) {
          lua_pushinteger(L, db_9x[i].callback_type1(arg1, arg2, arg3, arg4,
                                                     arg5, arg6, arg7, arg8));
        }
        return 1;
      }
    }
  }
  return luaL_error(L, "unknown syscall.");
}

static int lua_readword(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1) return luaL_error(L, "wrong number of arguments.");
#endif
  u32 *word = (u32 *)luaL_checkinteger(L, 1);
  lua_pushinteger(L, *word);
  return 1;
}

static int lua_storeword(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 2) return luaL_error(L, "wrong number of arguments.");
#endif
  u32 *offs = (u32 *)luaL_checkinteger(L, 1);
  u32 word = (u32)luaL_checkinteger(L, 2);
  *offs = word;
  return 0;
}

static int lua_getfh(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1) return luaL_error(L, "wrong number of arguments.");
#endif
  Handle hdl = luaL_checkinteger(L, 1);
  // fileStream *result = new fileStream;
  // result->isRomfs = false;
  // result->handle = (u32)hdl;
  // result->magic = 0xBEEFDEAD;
  // lua_pushinteger(L, (u32)result);
  return 1;
}

static int lua_alloc(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1) return luaL_error(L, "wrong number of arguments.");
#endif
  u32 size = (u32)luaL_checkinteger(L, 1);
  u8 *memblock = new u8[size];
  lua_pushinteger(L, (u32)memblock);
  return 1;
}

static int lua_alloc2(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1) return luaL_error(L, "wrong number of arguments.");
#endif
  u32 size = (u32)luaL_checkinteger(L, 1);
  u8 *memblock = (u8 *)linearAlloc(size);
  lua_pushinteger(L, (u32)memblock);
  return 1;
}

static int lua_free(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1) return luaL_error(L, "wrong number of arguments.");
#endif
  void *offset = (void *)luaL_checkinteger(L, 1);
  delete[] offset;
  return 0;
}

static int lua_free2(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1) return luaL_error(L, "wrong number of arguments.");
#endif
  void *offset = (void *)luaL_checkinteger(L, 1);
  linearFree(offset);
  return 0;
}

static int lua_getraw(lua_State *L) {
  int argc = lua_gettop(L);
#ifndef SKIP_ERROR_HANDLING
  if (argc != 1) return luaL_error(L, "wrong number of arguments.");
#endif
  wav *block = (wav *)luaL_checkinteger(L, 1);
  if (block->magic == 0x4C534E44) {
    lua_pushinteger(L, (u32)block->audiobuf);
    if (block->audiobuf2 != NULL)
      lua_pushinteger(L, (u32)block->audiobuf2);
    else
      lua_pushnil(L);
  } else
    return luaL_error(L, "unsupported memory block type.");
  return 2;
}

// Register our Core Functions
static const luaL_Reg Core_functions[] = {{"checkService", lua_service},
                                          {"execCall", lua_execall},
                                          {"getHandle", lua_getfh},
                                          {"getRawData", lua_getraw},
                                          {"readWord", lua_readword},
                                          {"storeWord", lua_storeword},
                                          {"free", lua_free},
                                          {"alloc", lua_alloc},
                                          {"linearFree", lua_free2},
                                          {"linearAlloc", lua_alloc2},
                                          {0, 0}};

void luaCore_init(lua_State *L) {
  lua_newtable(L);
  luaL_setfuncs(L, Core_functions, 0);
  lua_setglobal(L, "Core");
}