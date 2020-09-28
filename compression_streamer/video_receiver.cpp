#include "video_receiver.h"

#include <iostream>

namespace streamer {

    VideoReceiver::ReadRoutine::ReadRoutine(int socket_desc, bool& en, queue<Protocol::FrameDesc>& fifo, mutex &m)
        : _desc(socket_desc), _en(en), _fifo(fifo), _m(m) {
    }

    void VideoReceiver::ReadRoutine::operator() () {
        Protocol::FrameDesc frame_info;
        Protocol::Manager protocol;
        while (_en) {
            vector<uint8_t> buffer(36000);
            size_t n = recv(_desc, buffer.data(), buffer.size(), MSG_WAITALL);
            buffer.resize(n);
            bool _new_frame = protocol.handle_packet(buffer);
            if (_new_frame) {
                lock_guard<mutex> locker(_m);
                _fifo.push(std::move(protocol.read_data_chunk()));
            }
        }
    }

    VideoReceiver::~VideoReceiver() {
        close(_socket_desc);
    }

    VideoReceiver::VideoReceiver(string ip_rec, size_t udp_rec)
        : _receiver_ip(ip_rec), _receiver_udp(udp_rec) {
    }

    void VideoReceiver::SetAddress(string ip_rec, size_t udp_rec) {
        _receiver_ip = ip_rec;
        _receiver_udp = udp_rec;
    }

    void VideoReceiver::open_socket() {
        _socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
        if (_socket_desc == -1)
            throw std::runtime_error("Cant create socket");
        _in_address.sin_family = AF_INET;
        _in_address.sin_addr.s_addr = htonl(convert_addr(_receiver_ip));;
        _in_address.sin_port = _receiver_udp;

        int bind_res = bind(_socket_desc, (struct sockaddr *)&_in_address, sizeof (_in_address));
        if (bind_res == -1)
            throw std::runtime_error("Cant bind socket");
    }

    void VideoReceiver::Init() {
        open_socket();
    }

    void VideoReceiver::StartReceive() {
        _enable_loop = true;
        _thread_ptr = unique_ptr<thread>(new thread(ReadRoutine(_socket_desc, _enable_loop, _fifo, _mutex)));
    }

    void VideoReceiver::StopReceiver() {
        _enable_loop = false;
    }

    uint32_t VideoReceiver::convert_addr(string ip) {
        uint32_t out = 0;
        stringstream ss;
        ss << ip;

        unsigned int buf;
        char sep;
        for (size_t i = 0; i < 4; i++) {
            if (i != 0) {
                ss >> sep;
                if (sep!='.')
                    throw std::invalid_argument("Wrong IP-address for destination");
            }
            ss >> buf;
            out = out << 8;
            out |= buf;
        }

        return out;
    }

    size_t VideoReceiver::GetTraffic() {
        return _traffic.GetAverageTraffic();
    }

    Mat VideoReceiver::ReadFrame() {
        size_t fifo_sz = 0;
        Protocol::FrameDesc desc;
        bool ready=false;
        int type = CV_8UC4;
        while (!ready) {
            {
                lock_guard<mutex> locker(_mutex);
                fifo_sz = _fifo.size();
                if (fifo_sz) {
                    desc = std::move(_fifo.front());
                    _fifo.pop();
                    ready = true;
                }
            }
            if (!ready)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        _traffic.AddTransaction(desc.payload.size());
        Mat out_frame;
        if (desc.compression) {
            out_frame = _codec.decode(desc.payload);
        } else {
            switch (desc.pixel_size) {
                case 1: type = CV_8UC1; break;
                case 2: type = CV_8UC2; break;
                case 3: type = CV_8UC3; break;
                case 4: type = CV_8UC4; break;
                default: throw runtime_error("Unexpected pixel size"); break;
            }
            Mat buf = Mat(desc.img_sz_y, desc.img_sz_x, type, desc.payload.data());
            out_frame = buf.clone();
        }
        return out_frame;
    }

    VideoReceiver& operator>>(VideoReceiver& rec, Mat& frame) {
        frame = std::move(rec.ReadFrame());
        return rec;
    }

    void config_videoreceiver(VideoReceiver& rec, const Configurator& config) {
        rec.SetAddress(config.GetIp(), config.GetUdp());
    }


}
