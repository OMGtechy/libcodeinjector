cmake_minimum_required(VERSION 3.16)
project(codeinjector VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

option(ENABLE_ASAN "Enable address sanitizer flags." OFF)
option(ENABLE_UBSAN "Enable undefined behaviour sanitizer flags." OFF)

add_library(
    codeinjector
    source/inject.cpp
)

target_include_directories(
    codeinjector
    PUBLIC
    include
)

target_compile_options(
    codeinjector
    PRIVATE
    -fPIC
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

if(ENABLE_ASAN OR ENABLE_UBSAN)
    target_compile_options(
        codeinjector
        PUBLIC
        -fno-omit-frame-pointer
    )
endif()

if(ENABLE_ASAN)
    target_compile_options(
        codeinjector
        PUBLIC
        -fsanitize=address
    )
    target_link_options(
        codeinjector
        PUBLIC
        -fsanitize=address
    )
endif()

if(ENABLE_UBSAN)
    target_compile_options(
        codeinjector
        PUBLIC
        -fsanitize=undefined
    )
    target_link_options(
        codeinjector
        PUBLIC
        -fsanitize=undefined
    )
endif()

add_subdirectory(test)

