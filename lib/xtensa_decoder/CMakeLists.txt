cmake_minimum_required(VERSION 3.20)

project(binaryninja-xtensa-decoder)

set(DECODER_SOURCES
)

add_library(xtensa_decoder STATIC
    src/decoder.cpp
    src/decoder_wide.cpp
    src/decoder_narrow.cpp
    src/operands.cpp
    src/mnemonics.cpp
)

target_include_directories(xtensa_decoder PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
