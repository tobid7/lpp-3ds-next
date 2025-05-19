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

dkp_add_embedded_binary_library(${PROJECT_NAME}-data
    data/hbkb_abc_lower.bin
    data/hbkb_abc_upper.bin
    data/hbkb_back_sel.bin
    data/hbkb_caps_sel.bin
    data/hbkb_normal_sel.bin
    data/hbkb_shift_sel.bin
    data/hbkb_space_sel.bin
    data/hbkb_special_sel.bin
    data/hbkb_special.bin
)

# Add Lua
add_subdirectory(vendor)

add_executable(${PROJECT_NAME}
    source/main.cpp
    source/ErrorHelper.cpp
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

    # FTP
    source/include/ftp/ftp_cmd.cpp
    source/include/ftp/ftp.cpp

    # HB Kbd (Trashy Keyboard)
    source/include/hbkb/hbkb.cpp
    source/include/hbkb/hw_controls.cpp
    source/include/hbkb/hw_graphics.cpp
    build/.dkp-generated/lpp-3ds-next-data/hbkb_shift_sel_bin.S
    build/.dkp-generated/lpp-3ds-next-data/hbkb_special_sel_bin.S
    build/.dkp-generated/lpp-3ds-next-data/hbkb_abc_lower_bin.S
    build/.dkp-generated/lpp-3ds-next-data/hbkb_space_sel_bin.S
    build/.dkp-generated/lpp-3ds-next-data/hbkb_normal_sel_bin.S
    build/.dkp-generated/lpp-3ds-next-data/hbkb_special_bin.S
    build/.dkp-generated/lpp-3ds-next-data/hbkb_back_sel_bin.S
    build/.dkp-generated/lpp-3ds-next-data/hbkb_caps_sel_bin.S
    build/.dkp-generated/lpp-3ds-next-data/hbkb_abc_upper_bin.S

    # lodepng
    source/include/lodepng/lodepng.cpp

    # ttf
    source/include/ttf/Font.cpp
    source/include/ttf/Image.cpp

    # Core Base (Could be completly removed)
    source/include/Archive.cpp
    source/include/audio.cpp
    source/include/Graphics.cpp
    source/include/memory.cpp
    source/include/NDS.cpp
    source/include/utils.cpp
    source/include/vshader.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    source/include
    source/include/hbkb
    source/include/ttf
    source/include/lodepng
    $ENV{DEVKITPRO}/portlibs/3ds/include
    ${CMAKE_BINARY_DIR}/.dkp-generated/${PROJECT_NAME}-data)

target_link_libraries(${PROJECT_NAME} PUBLIC
    lua m z jpeg citro2d citro3d ctru)

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