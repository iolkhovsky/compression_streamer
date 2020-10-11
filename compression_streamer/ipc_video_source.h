#pragma once

#include "video_source.h"
#include "ipc_manager.h"
#include "stream_timing_stat.h"
#include "stream_traffic_stat.h"

#include <string>

namespace streamer {
    class IpcVideoSource: public IVideoSource {
    public:
        IpcVideoSource(std::string shmem_name, std::string sem_name, std::string mq_name);
        cv::Mat Read() override;
        double GetFps() const override;
        double GetTraffic() override;
    private:
        ipc::IpcManager _ipc;
        statistics::TimingStat _timing;
        statistics::TrafficStat _traffic;
    };
}
