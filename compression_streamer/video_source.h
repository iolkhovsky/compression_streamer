#pragma once

#include <sstream>
#include <string>
#include <stdexcept>

#include <opencv2/opencv.hpp>
#include "stream_timing_stat.h"
#include "stream_traffic_stat.h"

using cv::Mat;
using cv::VideoCapture;
using std::string;
using std::stringstream;
using std::deque;
using std::chrono::steady_clock;

class VideoSource {
    using timestamp = steady_clock::time_point;

public:
    VideoSource() = default;
    template<typename T>
    VideoSource(T id);
    template<typename T>
    bool Open(T id);
    Mat Read();
    double GetFps() const;
    double GetTraffic();

private:
    VideoCapture _cap;
    string _id;
    Mat _buffer;
    StreamTimingStat _timing_stat;
    StreamTrafficStat _traffic_stat;
};

void operator>>(VideoSource &src, Mat& dst);

template<typename T>
VideoSource::VideoSource(T id) {
    Open(id);
}

template<typename T>
bool VideoSource::Open(T id) {
    if (_cap.open(id)) {
        stringstream ss;
        ss << id;
        _id = ss.str();
    } else {
        throw std::invalid_argument("Invalid source (file path or web-camera id)");
    }
}
