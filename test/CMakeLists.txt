cmake_minimum_required(VERSION 3.21)
project(codeinjector-test LANGUAGES CXX)

option(ENABLE_ASAN "Enable address sanitizer flags." OFF)
option(ENABLE_UBSAN "Enable undefined behaviour sanitizer flags." OFF)

add_library(common INTERFACE)

if(ENABLE_ASAN OR ENABLE_UBSAN)
    target_compile_options(
        common
        INTERFACE
        -fno-omit-frame-pointer
    )
endif()

if(ENABLE_ASAN)
    target_compile_options(
        common
        INTERFACE
        -fsanitize=address
    )
    target_link_options(
        common
        INTERFACE
        -fsanitize=address
    )
endif()

if(ENABLE_UBSAN)
    target_compile_options(
        common
        INTERFACE
        -fsanitize=undefined
    )
    target_link_options(
        common
        INTERFACE
        -fsanitize=undefined
    )
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

target_compile_options(
    codeinjector-caller PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

target_link_libraries(
    codeinjector-caller
    common
    codeinjector
)

target_link_libraries(injected-library common)
target_link_libraries(injection-target common)

add_test(NAME codeinjector-test
         COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/source/codeinjector-test.py)

