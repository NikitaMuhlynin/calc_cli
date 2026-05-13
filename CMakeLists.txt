cmake_minimum_required(VERSION 3.20)

project(calc_cli VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    safe_math
    GIT_REPOSITORY https://github.com/NikitaMuhlynin/safe_math.git
    GIT_TAG homework-2
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.3
)

FetchContent_MakeAvailable(
    safe_math
    nlohmann_json
    googletest
    spdlog
)

find_package(PostgreSQL REQUIRED)

# -------------------- source lists --------------------

set(APP_SOURCES
    src/application/calculator.cpp
    src/application/context.cpp
    src/application/logger.cpp
    src/application/parser.cpp
    src/application/printer.cpp
    src/application/runner.cpp
)

set(CACHE_SERVICE_SOURCES
    src/cache_service/memory_cache.cpp
    src/cache_service/storage.cpp
)

set(PROJECT_LIBRARY_SOURCES
    ${APP_SOURCES}
    ${CACHE_SERVICE_SOURCES}
)

# -------------------- main executable --------------------

add_executable(
    calc_cli
    src/main.cpp
    ${PROJECT_LIBRARY_SOURCES}
)

target_include_directories(calc_cli PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(
    calc_cli
    PRIVATE
        safe_math::safe_math
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        ${PostgreSQL_LIBRARIES}
)

target_compile_options(calc_cli PRIVATE -Wall -Wextra -Wpedantic)

install(TARGETS calc_cli RUNTIME DESTINATION bin)

# -------------------- tests --------------------

enable_testing()

file(
    GLOB TEST_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
)

add_executable(
    calc_cli_tests
    ${TEST_SOURCES}
    ${PROJECT_LIBRARY_SOURCES}
)

target_include_directories(calc_cli_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(
    calc_cli_tests
    PRIVATE
        safe_math::safe_math
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        ${PostgreSQL_LIBRARIES}
        gtest
        gtest_main
)

target_compile_options(calc_cli_tests PRIVATE -Wall -Wextra -Wpedantic) 

add_test(NAME calc_cli_tests COMMAND calc_cli_tests)

# -------------------- valgrind tests --------------------

find_program(VALGRIND_EXE valgrind)

if(VALGRIND_EXE)
    add_test(
        NAME calc_cli_tests_valgrind
        COMMAND
            ${VALGRIND_EXE}
            --leak-check=full
            --show-leak-kinds=all
            --track-origins=yes
            --errors-for-leak-kinds=definite,indirect
            --error-exitcode=1
            $<TARGET_FILE:calc_cli_tests>
    )

    set_tests_properties(
        calc_cli_tests_valgrind
        PROPERTIES
            LABELS "valgrind"
            TIMEOUT 60
    )
endif()

# -------------------- source collection --------------------

file(
    GLOB_RECURSE PROJECT_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

# -------------------- formatting --------------------

find_program(CLANG_FORMAT_EXE clang-format)

if(CLANG_FORMAT_EXE)
    add_custom_target(format COMMAND "${CLANG_FORMAT_EXE}" -i ${PROJECT_SOURCES})

    add_custom_target(
        format-check
        COMMAND "${CLANG_FORMAT_EXE}" --dry-run --Werror ${PROJECT_SOURCES}
    )
endif()

# -------------------- clang-tidy --------------------

find_program(CLANG_TIDY_EXE clang-tidy)

if(CLANG_TIDY_EXE)
    set_target_properties(calc_cli PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    set_target_properties(calc_cli_tests PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

    add_custom_target(
        tidy
        COMMAND
            "${CLANG_TIDY_EXE}"
            ${PROJECT_SOURCES}
            --
            -I"${CMAKE_CURRENT_SOURCE_DIR}/include"
            -I"${safe_math_SOURCE_DIR}/include"
            -I"${nlohmann_json_SOURCE_DIR}/include"
            -I"${spdlog_SOURCE_DIR}/include"
            -I"${PostgreSQL_INCLUDE_DIRS}"
            -std=c++20
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()