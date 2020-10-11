#include "video_source.h"
#include "cv_video_source.h"
#include "ipc_video_source.h"
#include "configurator.h"

namespace streamer {
    std::shared_ptr<IVideoSource> make_video_source(const Configurator& conf) {
        switch(conf.GetSource()) {
        case Configurator::StreamSources::webcamera:
            return std::make_shared<CvVideoSource>(conf.GetWebcamId());
        case Configurator::StreamSources::videofile:
            return std::make_shared<CvVideoSource>(conf.GetVideoPath());
        case Configurator::StreamSources::ipc:
            return std::make_shared<IpcVideoSource>(conf.GetShmem(), conf.GetSemaphore(), conf.GetMQueue());
        }
        throw std::runtime_error("make_video_source has reached end of fucntion");
    }

    void operator>>(std::shared_ptr<IVideoSource> src, cv::Mat& frame) {
        frame = src->Read();
    }
}
