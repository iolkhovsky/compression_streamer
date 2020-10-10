#pragma once

#include <string>

namespace ipc {
    class PosixShmem {
    public:
        PosixShmem(std::string name, int size);
        uint8_t* get_pointer() const;
    private:
        int _size;
        int _fd;
        uint8_t* _ptr;
    };
}
