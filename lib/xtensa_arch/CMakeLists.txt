cmake_minimum_required(VERSION 3.20)

project(binaryninja-xtensa-arch C CXX)

add_library(arch_xtensa SHARED
    src/xtensa_arch.cpp
    src/xtensa_disasm.cpp
    src/xtensa_il_core_arithmetic.cpp
    src/xtensa_il_core_branch.cpp
    src/xtensa_il_core_call.cpp
    src/xtensa_il_core_jump.cpp
    src/xtensa_il_core_logical.cpp
    src/xtensa_il_core_mem_load.cpp
    src/xtensa_il_core_mem_order.cpp
    src/xtensa_il_core_mem_store.cpp
    src/xtensa_il_core_move.cpp
    src/xtensa_il_core_pc.cpp
    src/xtensa_il_core_shift.cpp
    src/xtensa_il_core.cpp
    src/xtensa_il_opt_boolean.cpp
    src/xtensa_il_opt_cond_store.cpp
    src/xtensa_il_opt_depbits.cpp
    src/xtensa_il_opt_exception2.cpp
    src/xtensa_il_opt_fp.cpp
    src/xtensa_il_opt_interrupt.cpp
    src/xtensa_il_opt_loop.cpp
    src/xtensa_il_opt_misc.cpp
    src/xtensa_il_opt_muldiv.cpp
    src/xtensa_il_opt_narrow.cpp
    src/xtensa_il_opt_sync.cpp
    src/xtensa_il_opt_windowed.cpp
    src/xtensa_il.cpp
)

if(WIN32)
    get_target_property(BN_API_SOURCE_DIR binaryninjaapi SOURCE_DIR)
    list(APPEND CMAKE_MODULE_PATH "${BN_API_SOURCE_DIR}/cmake")
    find_package(BinaryNinjaCore REQUIRED)
    target_link_libraries(arch_xtensa 
        xtensa_decoder 
        ${BinaryNinjaCore_LIBRARIES}
        binaryninjaapi)
    target_link_directories(arch_xtensa PRIVATE ${BinaryNinjaCore_LIBRARY_DIRS})
else()
    target_link_libraries(arch_xtensa xtensa_decoder binaryninjaapi)
endif()

target_include_directories(arch_xtensa PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../xtensa_decoder/include
)

set_target_properties(arch_xtensa PROPERTIES
    CXX_STANDARD 20
    CXX_VISIBILITY_PRESET hidden
    CXX_STANDARD_REQUIRED ON
    VISIBILITY_INLINES_HIDDEN ON
    POSITION_INDEPENDENT_CODE ON
)
