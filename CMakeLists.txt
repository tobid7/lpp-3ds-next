cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
enable_language(ASM)

project(lpp-3ds-next)

find_program(MAKEROM makerom)
find_program(BANNERTOOL bannertool)

# Set Result specific Data
set(APP_NAME "LPP3DS Next")
set(APP_DESC "Lua for the 3ds")
set(APP_AUTHOR "tobid7")
set(APP_ICON "${CMAKE_SOURCE_DIR}/app/icon.png")
set(APP_ROMFS "${CMAKE_SOURCE_DIR}/romfs")

set(APP_BANNER "${CMAKE_SOURCE_DIR}/app/banner.png")
set(APP_BANNERAUDIO "${CMAKE_SOURCE_DIR}/app/BannerAudio.wav")
set(APP_RSF "${CMAKE_SOURCE_DIR}/app/build-cia.rsf")
set(APP_LOGO "${CMAKE_SOURCE_DIR}/app/splash.lz")

# Core and Lua
add_subdirectory(lpp-next-core)

add_executable(${PROJECT_NAME}
    source/main.cpp
    source/lua_Camera.cpp
    source/lua_Controls.cpp
    source/lua_Core.cpp
    source/lua_Graphics.cpp
    source/lua_Keyboard.cpp
    source/lua_Mic.cpp
    source/lua_Network.cpp
    source/lua_Render.cpp
    source/lua_Screen.cpp
    source/lua_System.cpp
    source/lua_Timer.cpp
    source/luaPlayer.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    source/include/hbkb
    $ENV{DEVKITPRO}/portlibs/3ds/include
    ${CMAKE_BINARY_DIR}/.dkp-generated/${PROJECT_NAME}-data)

target_link_libraries(${PROJECT_NAME} PUBLIC
    hbkb lpp-next-core)

# Generate 3DSX
ctr_generate_smdh(
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.smdh
    NAME "${APP_NAME}"
    DESCRIPTION "${APP_DESC}"
    AUTHOR "${APP_AUTHOR}"
    ICON "${APP_ICON}"
)
ctr_create_3dsx(
    ${PROJECT_NAME}
    OUTPUT "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.3dsx"
    SMDH "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.smdh"
    ROMFS "${APP_ROMFS}"
    DEPENDS create_resources
)


add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bnr
    COMMAND ${BANNERTOOL} makebanner
    -i ${APP_BANNER}
    -a ${APP_BANNERAUDIO}
    -o ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bnr
    DEPENDS ${APP_BANNER} ${APP_BANNERAUDIO}
    COMMENT "Creating Banner for cia file..."
    VERBATIM
)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.cia
    COMMAND ${MAKEROM}
    -f cia -target t -exefslogo
    -o "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.cia"
    -elf "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf"
    -rsf ${APP_RSF}
    -banner "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bnr"
    -icon "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.smdh"
    -logo ${APP_LOGO} -DAPP_ROMFS="${APP_ROMFS}"
    -major 1 -minor 0 -micro 0
    -DAPP_VERSION_MAJOR=1
    DEPENDS ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.smdh
    COMMENT "Creating Cia file..."
    VERBATIM
)

add_custom_target(make_banner ALL
    DEPENDS ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bnr
)

add_custom_target(make_cia ALL
    DEPENDS ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bnr ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.cia
)