#pragma once

#include "posix_shmem.h"
#include "posix_semaphore.h"
#include "posix_mqueue.h"
#include <opencv2/opencv.hpp>

#include <string>

namespace ipc {
    class IpcManager {
    public:
        IpcManager(std::string shmem_name, std::string sem_name, std::string mq_name, bool blocking = true);
        void write_frame(const cv::Mat& img) const;
        cv::Mat read_frame() const;

        static constexpr int ShmemDedfaultSize = 2500 * 2000 * 3;
    private:
        PosixShmem _shm;
        PosixSemaphore _sem;
        PosixMQueue _mq;
    };

    void operator>>(const cv::Mat& img, IpcManager& ipc);
    void operator>>(IpcManager& ipc, cv::Mat& img);
}
