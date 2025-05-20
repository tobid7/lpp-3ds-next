cmake_minimum_required(VERSION 3.22)

project(hbkb)

enable_language(ASM)

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

add_library(hbkb STATIC
    source/hw_controls.cpp
    source/hw_graphics.cpp
    source/hbkb.cpp

    ${CMAKE_CURRENT_BINARY_DIR}/.dkp-generated/${PROJECT_NAME}-data/hbkb_special_sel_bin.S
    ${CMAKE_CURRENT_BINARY_DIR}/.dkp-generated/${PROJECT_NAME}-data/hbkb_shift_sel_bin.S
    ${CMAKE_CURRENT_BINARY_DIR}/.dkp-generated/${PROJECT_NAME}-data/hbkb_abc_lower_bin.S
    ${CMAKE_CURRENT_BINARY_DIR}/.dkp-generated/${PROJECT_NAME}-data/hbkb_space_sel_bin.S
    ${CMAKE_CURRENT_BINARY_DIR}/.dkp-generated/${PROJECT_NAME}-data/hbkb_normal_sel_bin.S
    ${CMAKE_CURRENT_BINARY_DIR}/.dkp-generated/${PROJECT_NAME}-data/hbkb_special_bin.S
    ${CMAKE_CURRENT_BINARY_DIR}/.dkp-generated/${PROJECT_NAME}-data/hbkb_back_sel_bin.S
    ${CMAKE_CURRENT_BINARY_DIR}/.dkp-generated/${PROJECT_NAME}-data/hbkb_caps_sel_bin.S
    ${CMAKE_CURRENT_BINARY_DIR}/.dkp-generated/${PROJECT_NAME}-data/hbkb_abc_upper_bin.S
)

add_dependencies(hbkb ${PROJECT_NAME}-data)

target_include_directories(hbkb PUBLIC
    include
    ${CMAKE_CURRENT_BINARY_DIR}/.dkp-generated/${PROJECT_NAME}-data
)

target_link_libraries(hbkb PUBLIC
    m ctru
)