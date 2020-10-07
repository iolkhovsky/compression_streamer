#include "posix_shmem.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>


namespace ipc {
    PosixShmem::PosixShmem(std::string name, int size) {
        _size = size;
        //_fd = shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRWXO);
        _fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
        if (_fd <= 0)
            throw std::runtime_error("Invalid shmem file descriptor");
        ftruncate(_fd, _size);
        _ptr = (uint8_t*)mmap(NULL, static_cast<size_t>(_size), PROT_READ|PROT_WRITE, MAP_SHARED, _fd, 0);
    }

    uint8_t* PosixShmem::get_pointer() const {
        return _ptr;
    }
}
