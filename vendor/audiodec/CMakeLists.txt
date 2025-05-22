cmake_minimum_required(VERSION 3.22)

project(audiodec)

add_library(audiodec STATIC
    source/audio_decoder.cpp
    source/decoder_mpg123.cpp
)

target_include_directories(audiodec PUBLIC
    include
)

target_link_libraries(audiodec PUBLIC
    m mpg123 vorbisidec ogg ctru
)