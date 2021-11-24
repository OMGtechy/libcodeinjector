cmake_minimum_required(VERSION 3.21)
project(codeinjector-test LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    catch
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.7
)

FetchContent_MakeAvailable(catch)

option(ENABLE_ASAN "Enable address sanitizer flags." OFF)
option(ENABLE_UBSAN "Enable undefined behaviour sanitizer flags." OFF)

add_executable(
    codeinjector-test
    source/codeinjector-test.cpp
)

target_compile_options(
    codeinjector-test PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

target_include_directories(
    codeinjector-test PRIVATE
    include
)

target_link_libraries(
    codeinjector-test
    Catch2::Catch2
    codeinjector
)

if(ENABLE_ASAN OR ENABLE_UBSAN)
    target_compile_options(
        codeinjector-test
        PRIVATE
        -fno-omit-frame-pointer
    )
endif()

if(ENABLE_ASAN)
    target_compile_options(
        codeinjector-test
        PRIVATE
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
        codeinjector-test
        PRIVATE
        -fsanitize=undefined
    )
    target_link_options(
        codeinjector
        PUBLIC
        -fsanitize=undefined
    )
endif()

add_test(
    NAME codeinjector-test
    COMMAND $<TARGET_FILE:codeinjector-test>
)

