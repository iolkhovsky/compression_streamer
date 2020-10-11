#include "ipc_video_source.h"

namespace streamer {
    IpcVideoSource::IpcVideoSource(std::string shmem_name, std::string sem_name, std::string mq_name)
        : _ipc(shmem_name, sem_name, mq_name) {
    }

    cv::Mat IpcVideoSource::Read() {
        cv::Mat frame;
        _ipc >> frame;
        _timing.PushEvent();
        _traffic.AddTransaction(frame.total()*frame.elemSize());
        return frame;
    }

    double IpcVideoSource::GetFps() const {
        return _timing.GetFps();
    }

    double IpcVideoSource::GetTraffic() {
        return _traffic.GetAverageTraffic();
    }
}
