#include "video_receiver.h"

#include <iostream>

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
    listen();
}

void VideoReceiver::StopReceiver() {
    _enable_loop = false;
}

void VideoReceiver::listen() {
    vector<uchar> buffer(2048);
    while (_enable_loop) {
        size_t n = recv(_socket_desc, buffer.data(), buffer.size(), MSG_WAITALL);
        std::cout << "Recieved: " << n << std::endl;
    }
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


void operator>>(VideoReceiver& rec, Mat& frame) {

}
