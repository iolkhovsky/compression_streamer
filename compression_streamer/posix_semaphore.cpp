#include "posix_semaphore.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <stdexcept>

#include <iostream>

namespace ipc {
    PosixSemaphore::PosixSemaphore(std::string name) {
        try {
            _sem = sem_open(name.c_str(), O_CREAT, S_IRWXO, 1);
            if (_sem == SEM_FAILED)
                throw std::runtime_error("Semaphore's opening faield");
        } catch (std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
    }

    void PosixSemaphore::lock() const {
        if (_sem)
            sem_wait(_sem);
        else
            throw std::runtime_error("Try to lock semaphore while it hasn't been opened");
    }

    void PosixSemaphore::unlock() const {
        if (_sem)
            sem_post(_sem);
        else
            throw std::runtime_error("Try to lock semaphore while it hasn't been opened");
    }
}
