#include <codeinjector/inject.hpp>

using namespace codeinjector;

int main() {
    // The C API invokes the C++ API,
    // so using the C API in the test gives us better coverage.
    const char* args[] = {
        "a", "b", "c"
    };

    codeinjector_inject_library("./injection-target", args, std::size(args), "libinjected-library.so");

    // Should only ever get here if injecting the library failed.
    return -1;
}

