#include "codeinjector/inject.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>

#include <unistd.h>

extern char** environ;

namespace {
    char* to_cstr(std::string_view str) {
        char* const cstr = strndup(str.data(), str.size());
        assert(cstr != nullptr);
        return cstr;
    }

    char* duplicate_cstr(const char* const oldCStr) {
        char* const newCStr = strdup(oldCStr);
        assert(newCStr != nullptr);
        return newCStr;
    }

    std::vector<char*> get_environment() {
        std::vector<char*> environment;

        for (size_t i = 0; environ[i] != nullptr; ++i) {
            environment.push_back(duplicate_cstr(environ[i]));
        }

        environment.push_back(nullptr);
        return environment;
    }

    std::vector<char*> append_ld_preload(std::vector<char*> environment, std::filesystem::path lib) {
        constexpr std::string_view ldPreloadPrefix = "LD_PRELOAD=";
        const auto libPath = std::filesystem::absolute(lib).string();

        auto existingLDPreloadIter = std::ranges::find_if(
            environment,
            [ldPreloadPrefix](char* const cstr) {
                return cstr != nullptr && strncmp(cstr, ldPreloadPrefix.data(), ldPreloadPrefix.size()) == 0;
            }
        );

        if (existingLDPreloadIter != environment.end()) {
            std::string ldPreload = *existingLDPreloadIter;
            ldPreload.reserve(ldPreload.size() + 1 /* space */ + libPath.size() + 1 /* null terminator */);
            ldPreload.append(" ");
            ldPreload.append(libPath);

            free(*existingLDPreloadIter);
            *existingLDPreloadIter = to_cstr(ldPreload);
        } else {
            environment.insert(environment.end() - 2 /* skip last entry */, to_cstr(std::string(ldPreloadPrefix) + libPath));
        }

        return environment;
    }
}

bool codeinjector::inject_library(std::filesystem::path program, std::vector<std::string_view> args, std::filesystem::path lib) {
    assert(std::filesystem::exists(program));
    assert(std::filesystem::exists(lib));

    std::vector<char*> argv;
    argv.reserve(1 /* target program */ + args.size() + 1 /* nullptr */);

    argv.push_back(to_cstr(program.operator std::string()));
    std::transform(args.cbegin(), args.cend(), std::back_inserter(argv), to_cstr);
    argv.push_back(nullptr);

    assert(argv.size() >= 2);

    auto environment = append_ld_preload(get_environment(), lib.operator std::string());

    execvpe(argv[0], &argv[1], environment.data());

    // Should only ever get here if the exec failed
    for(auto* const cstr : argv) { free(cstr); }
    for(auto* const cstr : environment) { free(cstr); }
    return false;
}

extern "C" {
    bool codeinjector_inject_library(const char* const program,
                                     const char** const cArgs, size_t cArgsCount,
                                     const char* lib) {
        std::vector<std::string_view> args;
        args.resize(cArgsCount);

        for (size_t i = 0; i < cArgsCount; ++i) {
            args[i] = cArgs[i];
        }

        return codeinjector::inject_library(program, args, lib);
    }
}

