#pragma once

#include <string>
#include <sstream>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdexcept>

#include <opencv2/opencv.hpp>
#include "stream_timing_stat.h"
#include "stream_traffic_stat.h"

using cv::Mat;
using std::string;
using std::stringstream;

class VideoReceiver {
public:
    VideoReceiver() = default;
    VideoReceiver(string ip_rec, size_t udp_rec);
    void SetAddress(string ip_rec, size_t udp_rec);
    void StartReceive();
    void Init();
    size_t GetTraffic();
private:
    string _receiver_ip;
    size_t _receiver_udp;
    int _socket_desc;
    sockaddr_in _in_address;
    Mat _image_buffer;
    StreamStatistics::TimingStat _timing;
    StreamStatistics::TrafficStat _traffic;

    bool open_socket();
    void listen();
    static uint32_t convert_addr(string ip);
};

void operator>>(VideoReceiver& rec, Mat& frame);
