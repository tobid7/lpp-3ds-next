cmake_minimum_required(VERSION 3.22)

add_subdirectory(hbkb)
add_subdirectory(audiodec)

project(lua)

# As git symply throws an error when the file is already ptched
# we can simply ignore that error
message(STATUS "Applying lua patch...")
execute_process(
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR}/lua
    git apply ${CMAKE_CURRENT_SOURCE_DIR}/patches/lua.patch
    RESULT_VARIABLE PATCH_RESULT
)

add_library(lua STATIC
    lua/lapi.c
    lua/lauxlib.c
    lua/lbaselib.c
    lua/lcode.c
    lua/lcode.h
    lua/lcorolib.c
    lua/lctype.c
    lua/lctype.h
    lua/ldblib.c
    lua/ldebug.c
    lua/ldo.c
    lua/ldump.c
    lua/lfunc.c
    lua/lfunc.h
    lua/lgc.c
    lua/lgc.h
    lua/linit.c
    lua/liolib.c
    lua/llex.c
    lua/lmathlib.c
    lua/lmem.c
    lua/loadlib.c
    lua/lobject.c
    lua/lobject.h
    lua/lopcodes.c
    lua/lopcodes.h
    lua/loslib.c
    lua/lparser.c
    lua/lstate.c
    lua/lstring.c
    lua/lstrlib.c
    lua/ltable.c
    lua/ltablib.c
    lua/ltests.c
    lua/ltm.c
    lua/lua.c
    lua/luaconf.h
    lua/lundump.c
    lua/lutf8lib.c
    lua/lvm.c
    lua/lzio.c
)

target_include_directories(lua PUBLIC lua)