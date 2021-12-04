cmake_minimum_required(VERSION 3.21)
project(codeinjector-test LANGUAGES CXX)

option(ENABLE_ASAN "Enable address sanitizer flags." OFF)
option(ENABLE_UBSAN "Enable undefined behaviour sanitizer flags." OFF)

list(
    APPEND TEST_COMPILE_OPTIONS
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

if(ENABLE_ASAN OR ENABLE_UBSAN)
    list(APPEND TEST_COMPILE_OPTIONS -fno-omit-frame-pointer)
endif()

if(ENABLE_ASAN)
    list(APPEND TEST_COMPILE_OPTIONS -fsanitize=address)
    list(APPEND TEST_LINK_OPTIONS -fsanitize=address)
endif()

if(ENABLE_UBSAN)
    list(APPEND TEST_COMPILE_OPTIONS -fsanitize=undefined)
    list(APPEND TEST_LINK_OPTIONS -fsanitize=undefined)
endif()

add_library(
    injected-library SHARED
    source/injected-library.cpp
)

add_executable(
    injection-target
    source/injection-target.cpp
)

add_executable(
    codeinjector-caller
    source/codeinjector-caller.cpp
)

target_link_libraries(
    codeinjector-caller
    codeinjector
)

target_compile_options(injected-library    PRIVATE ${TEST_COMPILE_OPTIONS})
target_compile_options(injection-target    PRIVATE ${TEST_COMPILE_OPTIONS})
target_compile_options(codeinjector-caller PRIVATE ${TEST_COMPILE_OPTIONS})

target_link_options(injected-library    PRIVATE ${TEST_LINK_OPTIONS})
target_link_options(injection-target    PRIVATE ${TEST_LINK_OPTIONS})
target_link_options(codeinjector-caller PRIVATE ${TEST_LINK_OPTIONS})

add_test(NAME codeinjector-test
         COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/source/codeinjector-test.py ${ENABLE_ASAN})

