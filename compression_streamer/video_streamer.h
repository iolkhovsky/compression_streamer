#pragma once

#include <string>
#include <sstream>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>

#include <opencv2/opencv.hpp>
#include "stream_timing_stat.h"
#include "stream_traffic_stat.h"
#include "transport_protocol.h"

using cv::Mat;
using std::string;
using std::stringstream;

class VideoStreamer {
public:
    VideoStreamer() = default;
    ~VideoStreamer();
    VideoStreamer(string ip_dest, size_t udp_dest);
    void SetDestination(string ip_dest, size_t udp_dest);
    void SendFrame(const Mat& frame);
    void SendMessage(string msg);
    void Init();
    size_t GetTraffic();
private:
    string _destination_ip;
    size_t _destination_udp;
    int _socket_desc;
    sockaddr_in _in_address;
    sockaddr_in _dest_address;
    Mat _image_buffer;
    StreamStatistics::TimingStat _timing;
    StreamStatistics::TrafficStat _traffic;
    Protocol::Manager _protocol;

    bool open_socket();
    size_t send_packet(const char *buf, size_t sz);
    size_t send_packet(uint8_t *buf, size_t sz);
    void update_destination();
    static uint32_t convert_addr(string ip);
};

void operator>>(const Mat& frame, VideoStreamer& streamer);
