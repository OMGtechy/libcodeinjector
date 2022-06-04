#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

namespace codeinjector {
    //!
    //! \brief  Injects a library into a program.
    //!
    //! \param[in]  program  The program to inject the library into.
    //! \param[in]  args     The program arguments.
    //! \param[in]  lib      The library to inject.
    //!
    //! \returns  On failure only.
    //!
    //! \note  The current program will be replaced by the one specified, so you may want to fork before calling this.
    //!
    bool inject_library(std::filesystem::path program, std::vector<std::string_view> args, std::filesystem::path lib);
}

extern "C" {
    //!
    //! \brief  C wrapper for codeinjector::inject_library.
    //!
    //! \note  All const char* strings must be null terminated.
    //!
    bool codeinjector_inject_library(const char* program,
                                     const char** args, size_t argsCount,
                                     const char* lib);
}

