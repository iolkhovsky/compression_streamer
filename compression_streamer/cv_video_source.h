#pragma once

#include "stream_timing_stat.h"
#include "stream_traffic_stat.h"
#include "video_source.h"

#include <opencv2/opencv.hpp>

#include <sstream>
#include <string>
#include <stdexcept>

using cv::Mat;
using cv::VideoCapture;
using std::string;
using std::stringstream;
using std::deque;
using std::chrono::steady_clock;

namespace streamer {

    class CvVideoSource: public IVideoSource {
        using timestamp = steady_clock::time_point;

    public:
        CvVideoSource() = default;
        template<typename T>
        CvVideoSource(T id);
        template<typename T>
        void Open(T id);
        Mat Read() override;
        double GetFps() const override;
        double GetTraffic() override;

    private:
        VideoCapture _cap;
        string _id;
        Mat _buffer;
        statistics::TimingStat _timing;
        statistics::TrafficStat _traffic;
    };

    template<typename T>
    CvVideoSource::CvVideoSource(T id) {
        Open(id);
    }

    template<typename T>
    void CvVideoSource::Open(T id) {
        if (_cap.open(id)) {
            stringstream ss;
            ss << id;
            _id = ss.str();
        } else {
            throw std::invalid_argument("Invalid source (file path or web-camera id)");
        }
    }

}
