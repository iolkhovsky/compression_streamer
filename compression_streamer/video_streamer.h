#pragma once

#include <string>

#include <opencv2/opencv.hpp>
#include "stream_timing_stat.h"

using cv::Mat;
using std::string;

class VideoStreamer {
public:
    VideoStreamer() = default;
    VideoStreamer(string ip_dest, size_t udp_dest);
    void SetDestination(string ip_dest, size_t udp_dest);
    void SendFrame(const Mat& frame);
private:
    StreamTimingStat _time_stat;
};

void operator>>(const Mat& frame, VideoStreamer& streamer);
