#pragma once

#include <string>
#include <sstream>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdexcept>
#include <unistd.h>
#include <thread>

#include <opencv2/opencv.hpp>
#include "stream_timing_stat.h"
#include "stream_traffic_stat.h"
#include "transport_protocol.h"
#include "video_codec.h"
#include "configurator.h"

using cv::Mat;
using std::string;
using std::stringstream;

namespace streamer {

    class VideoStreamer {
    public:
        VideoStreamer() = default;
        ~VideoStreamer();
        VideoStreamer(string ip_dest, size_t udp_dest);
        void SetDestination(string ip_dest, size_t udp_dest);
        void SendFrame(const Mat& frame);
        void SendMessage(string msg);
        void Init();
        void SetCompression(int quality);
        size_t GetTraffic();
    private:
        string _destination_ip;
        size_t _destination_udp;
        int _socket_desc;
        sockaddr_in _in_address;
        sockaddr_in _dest_address;
        Mat _image_buffer;
        statistics::TimingStat _timing;
        statistics::TrafficStat _traffic;
        Protocol::Manager _protocol;
        int _compression;
        VideoCodec _codec;

        void open_socket();
        size_t send_packet(const char *buf, size_t sz);
        size_t send_packet(uint8_t *buf, size_t sz);
        void update_destination();
        static uint32_t convert_addr(string ip);
    };

    void operator>>(const Mat& frame, VideoStreamer& streamer);
    void config_videostreamer(VideoStreamer& streamer, const Configurator& configurator);

}
