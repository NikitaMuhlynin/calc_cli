cmake_minimum_required(VERSION 3.20)

project(calc_cli VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    safe_math
    GIT_REPOSITORY https://github.com/NikitaMuhlynin/safe_math.git
    GIT_TAG main
)

FetchContent_MakeAvailable(safe_math)

add_executable(
    calc_cli
    src/main.cpp
    src/runner.cpp
    src/parser.cpp
    src/calculator.cpp
    src/printer.cpp
)

target_include_directories(calc_cli PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(calc_cli PRIVATE safe_math::safe_math)
target_compile_options(calc_cli PRIVATE -Wall -Wextra -Wpedantic)

install(TARGETS calc_cli RUNTIME DESTINATION bin)

file(
    GLOB_RECURCE PROJECT_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

find_program(CLANG_FORMAT_EXE clang-format)

if(CLANG_FORMAT_EXE)
    add_custom_target(format COMMAND "${CLANG_FORMAT_EXE}" -i ${PROJECT_SOURCES})

    add_custom_target(
        format-check
        COMMAND "${CLANG_FORMAT_EXE}" --dry-run --Werror ${PROJECT_SOURCES}
    )
endif()

find_program(CLANG_TIDY_EXE clang-tidy)

if(CLANG_TIDY_EXE)
    set_target_properties(calc_cli PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

    add_custom_target(
        tidy
        COMMAND
            "${CLANG_TIDY_EXE}"
            ${PROJECT_SOURCES}
            --
            -I"${CMAKE_CURRENT_SOURCE_DIR}/include"
            -I"${safe_math_SOURCE_DIR}/include"
            -std=c++20
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()