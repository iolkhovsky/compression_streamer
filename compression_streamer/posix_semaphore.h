#pragma once

#include <string>
#include <semaphore.h>

namespace ipc {
    class PosixSemaphore {
    public:
        PosixSemaphore(std::string name);

        void lock() const;
        void unlock() const;
    private:
        sem_t* _sem;
    };
}




