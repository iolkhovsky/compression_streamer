#include "posix_semaphore.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <stdexcept>

namespace ipc {
    PosixSemaphore::PosixSemaphore(std::string name) {
        _sem = sem_open(name.c_str(), O_CREAT, S_IRWXO, 1);
        if (_sem == SEM_FAILED)
            throw std::runtime_error("Semaphore's opening faield");
    }

    void PosixSemaphore::lock() const {
        sem_wait(_sem);
    }

    void PosixSemaphore::unlock() const {
        sem_post(_sem);
    }
}
