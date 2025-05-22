#pragma once

// The NPI Lua Interface
#include <string>

#include "lua.hpp"

namespace D7 {
typedef lua_State* State;
class Arguments {
 public:
  Arguments(State state);
  ~Arguments();
  int num_arg();
  std::string get_string(int idx);
  int get_int(int idx);
  double get_number(int idx);
  bool get_bool(int idx);

 private:
  int argc;
  State istate;
};

class Table {
 public:
  Table(State state) {
    istate = state;
    lua_newtable(istate);
  }
  ~Table() {}

  void BeginSubTable(const std::string& name) {
    lua_pushstring(istate, name.c_str());
    lua_newtable(istate);
  }

  void EndSubTable() { lua_settable(istate, -3); }

  void number_field(const std::string& name, double num) {
    lua_pushstring(istate, name.c_str());
    lua_pushnumber(istate, num);
    lua_settable(istate, -3);
  }
  void int_field(const std::string& name, int num) {
    lua_pushstring(istate, name.c_str());
    lua_pushinteger(istate, num);
    lua_settable(istate, -3);
  }
  void string_field(const std::string& name, const std::string& str) {
    lua_pushstring(istate, name.c_str());
    lua_pushstring(istate, str.c_str());
    lua_settable(istate, -3);
  }
  void bool_field(const std::string& name, bool val) {
    lua_pushstring(istate, name.c_str());
    lua_pushboolean(istate, val);
    lua_settable(istate, -3);
  }

  void Flush() {
    luaL_getmetatable(istate, "naio_mtab");
    lua_setmetatable(istate, -2);
  }

 private:
  State istate;
};
class Lua {
 public:
  Lua();
  ~Lua();

 private:
  State LState;
};

// Template class for Functrion Interface
class LuaInterface {
 public:
  LuaInterface() = default;
  virtual ~LuaInterface() = default;
};
}  // namespace D7