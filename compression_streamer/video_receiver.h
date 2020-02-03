#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>
#include <thread>
#include <functional>
#include <queue>
#include <chrono>

#include <opencv2/opencv.hpp>
#include "stream_timing_stat.h"
#include "stream_traffic_stat.h"
#include "transport_protocol.h"
#include "synchronized.h"

using cv::Mat;
using std::string;
using std::stringstream;
using std::vector;
using std::unique_ptr;
using std::thread;

class VideoReceiver {
public:
    struct ReadRoutine {
        ReadRoutine(int socket_desc, bool& en, queue<Protocol::FrameDesc>& fifo, mutex &m);
        void operator() ();
    private:
        int _desc;
        bool& _en;
        queue<Protocol::FrameDesc>& _fifo;
        mutex &_m;
    };

    VideoReceiver() = default;
    ~VideoReceiver();
    VideoReceiver(string ip_rec, size_t udp_rec);
    void SetAddress(string ip_rec, size_t udp_rec);
    void StartReceive();
    void StopReceiver();
    void Init();
    size_t GetTraffic();
    Mat ReadFrame();
private:
    string _receiver_ip;
    size_t _receiver_udp;
    int _socket_desc;
    sockaddr_in _in_address;
    Mat _image_buffer;
    StreamStatistics::TimingStat _timing;
    StreamStatistics::TrafficStat _traffic;
    bool _enable_loop;
    unique_ptr<thread> _thread_ptr;
    queue<Protocol::FrameDesc> _fifo;
    mutex _mutex;

    void open_socket();
    static uint32_t convert_addr(string ip);
};

VideoReceiver& operator>>(VideoReceiver& rec, Mat& frame);
