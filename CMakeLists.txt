cmake_minimum_required(VERSION 3.10)

# Project
project(chromashell
    VERSION 1.0)

set(chromashell_VERSION_YEAR
    2022)

set(OUTPUT_NAME
    chromashell)

# Executable
set(chromashell_CODE_SOURCE_DIR
    ${chromashell_SOURCE_DIR}/src)

set(chromashell_SYSTEM_DIR
    share/chromashell)

configure_file(${chromashell_CODE_SOURCE_DIR}/config.h.in
    config.h)

set(chromashell_SOURCES
    chromashell.c
    color.c
    errmsg.c
    presets.c
    shell.c
    typecheck.c
    vstrcmp.c)

list(TRANSFORM chromashell_SOURCES
    PREPEND "${chromashell_CODE_SOURCE_DIR}/")

add_executable(${OUTPUT_NAME}
    ${chromashell_SOURCES})

target_include_directories(chromashell PUBLIC
    "${chromashell_BINARY_DIR}")

target_link_libraries(chromashell PRIVATE cjson)

# Post-build
add_custom_command(TARGET chromashell POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${chromashell_CODE_SOURCE_DIR}/presets.json
        ${CMAKE_CURRENT_BINARY_DIR})

# Install target
install(TARGETS chromashell
    RUNTIME
    DESTINATION bin)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/presets.json
    DESTINATION ${chromashell_SYSTEM_DIR})

# Uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)
endif()