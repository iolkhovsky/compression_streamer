#pragma once

#include "configurator.h"

#include <opencv2/opencv.hpp>

#include <memory>


namespace streamer {

    enum class SourceType {
      opencv_cap = 0x0,
      ipc_source = 0x1,
    };

    class IVideoSource {
    public:
        virtual cv::Mat Read() = 0;
        virtual double GetFps() const = 0;
        virtual double GetTraffic() = 0;
        virtual ~IVideoSource() = default;
    };

    void operator>>(std::shared_ptr<IVideoSource> src, cv::Mat& frame);
    std::shared_ptr<IVideoSource> make_video_source(const Configurator& conf);
}
