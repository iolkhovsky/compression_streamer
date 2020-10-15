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
#include "video_codec.h"
#include "configurator.h"

using cv::Mat;
using std::string;
using std::stringstream;
using std::vector;
using std::unique_ptr;
using std::thread;

namespace streamer {
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
        int GetInputTraffic();
        int GetOutputTraffic();
        double GetFPS();
        std::pair<Mat, double> ReadFrame();
    private:
        string _receiver_ip;
        size_t _receiver_udp;
        int _socket_desc;
        sockaddr_in _in_address;
        Mat _image_buffer;
        statistics::TimingStat _timing;
        statistics::TrafficStat _traffic;
        statistics::TrafficStat _out_traffic;
        bool _enable_loop;
        unique_ptr<thread> _thread_ptr;
        queue<Protocol::FrameDesc> _fifo;
        mutex _mutex;
        VideoCodec _codec;

        void open_socket();
        static uint32_t convert_addr(string ip);
    };

    VideoReceiver& operator>>(VideoReceiver& rec, std::pair<cv::Mat, double>& frame);
    void config_videoreceiver(VideoReceiver& rec, const Configurator& config);

}

