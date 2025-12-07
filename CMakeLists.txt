cmake_minimum_required(VERSION 3.20)

project(binaryninja-xtensa C CXX)

if(NOT BN_API_PATH)
    set(BN_API_PATH ${PROJECT_SOURCE_DIR}/submodules/binaryninja-api)
endif()
if(EXISTS ${BN_API_PATH})
    add_subdirectory(${BN_API_PATH} ${PROJECT_BINARY_DIR}/binaryninja-api)
else()
    message(FATAL_ERROR "Binary Ninja API not found at ${BN_API_PATH}")
endif()

add_subdirectory(lib/xtensa_decoder)
add_subdirectory(lib/xtensa_arch)
