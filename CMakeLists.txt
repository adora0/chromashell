cmake_minimum_required(VERSION 3.10)

# Project
project(chromashell VERSION 1.0)

set(chromashell_VERSION_YEAR 2022)

# Executable
set(chromashell_SYSTEM_DIR share/chromashell)

configure_file(config.h.in config.h)

add_executable(chromashell
    chromashell.c
    color.c
    typecheck.c
    shell.c
    errmsg.c
    presets.c
)

target_include_directories(chromashell PUBLIC
    "${PROJECT_BINARY_DIR}"
)

target_link_libraries(chromashell PRIVATE cjson)

# Post-build
add_custom_command(TARGET chromashell POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/presets.json
        $<TARGET_FILE_DIR:chromashell>
)

# Install target
install(TARGETS chromashell RUNTIME DESTINATION bin)
install(FILES presets.json DESTINATION ${chromashell_SYSTEM_DIR})

# Uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
        IMMEDIATE @ONLY
    )

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake
    )
endif()