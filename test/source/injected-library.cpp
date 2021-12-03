#include <fcntl.h>
#include <unistd.h>

namespace {
    __attribute__((constructor))
    void ctor() {
        close(creat("ctor", S_IRUSR | S_IWUSR));
    }
}
