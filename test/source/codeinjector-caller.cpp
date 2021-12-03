#include <codeinjector/inject.hpp>

using namespace codeinjector;

int main() {
    inject_library("./injection-target", {"a", "b", "c"}, "libinjected-library.so");

    // Should only ever get here if injecting the library failed.
    return -1;
}

