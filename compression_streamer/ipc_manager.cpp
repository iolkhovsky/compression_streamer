#include "ipc_manager.h"

#include <sstream>

namespace ipc {

    IpcManager::IpcManager(std::string shmem_name, std::string sem_name, std::string mq_name)
        : _shm(shmem_name, ShmemDedfaultSize),
        _sem(sem_name),
        _mq(mq_name) {
    }

    void IpcManager::write_frame(const cv::Mat& img) const {
        size_t frame_size = img.total() * img.elemSize();
        auto shmem_ptr = _shm.get_pointer();
        std::stringstream ss;
        ss << frame_size << " " << img.type() << " " << img.rows << " " << img.cols;
        _sem.lock();
        std::copy(img.data, img.data + frame_size, shmem_ptr);
        _sem.unlock();
        _mq.send(ss.str());
    }

    cv::Mat IpcManager::read_frame() const {
        std::string msg = _mq.receive();
        _sem.lock();
        std::stringstream ss(msg);
        size_t imgsz;
        int type, rows, cols;
        ss >> imgsz >> type >> rows >> cols;
        cv::Mat img(rows, cols, type, cv::Scalar(0));
        auto ptr = _shm.get_pointer();
        std::copy(ptr, ptr + imgsz, img.data);
        _sem.unlock();
        return img;
    }

    void operator>>(const cv::Mat& img, IpcManager& ipc) {
        ipc.write_frame(img);
    }

    void operator>>(IpcManager& ipc, cv::Mat& img) {
        img = ipc.read_frame();
    }

}
