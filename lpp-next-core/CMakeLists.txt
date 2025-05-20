cmake_minimum_required(VERSION 3.22)

project(lpp-next-core)

add_subdirectory(../vendor vendor)

add_library(lpp-next-core STATIC
    source/Archive.cpp
    source/audio.cpp
    source/ErrorHelper.cpp
    source/Filesystem.cpp
    source/Font.cpp
    source/ftp_cmd.cpp
    source/ftp.cpp
    source/Graphics.cpp
    source/Image.cpp
    source/utils.cpp
    source/vshader.cpp
)

target_include_directories(lpp-next-core PUBLIC
    include
    include/stb
    $ENV{DEVKITPRO}/portlibs/3ds/include
)

target_link_libraries(lpp-next-core PUBLIC
    m z jpeg lua citro2d citro3d ctru
)