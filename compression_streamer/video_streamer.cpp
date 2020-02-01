#include "video_streamer.h"

VideoStreamer::VideoStreamer(string ip_dest, size_t udp_dest)
    : _destination_ip(ip_dest), _destination_udp(udp_dest) {
    update_destination();
}

void VideoStreamer::SetDestination(string ip_dest, size_t udp_dest) {
    _destination_ip = ip_dest;
    _destination_udp = udp_dest;
    update_destination();
}

void VideoStreamer::update_destination() {
    _dest_address.sin_family = AF_INET;
    _dest_address.sin_addr.s_addr = convert_addr(_destination_ip);
    _dest_address.sin_port = _destination_udp;
}

uint32_t VideoStreamer::convert_addr(string ip) {
    uint32_t out = 0;
    stringstream ss;
    ss << ip;

    unsigned int buf;
    for (size_t i = 0; i < 4; i++) {
        if (i != 0)
            ss.ignore(1);
        ss >> buf;
        out |= (buf << i*8);
    }

    return out;
}

void VideoStreamer::SendFrame(const Mat& frame) {

}

void VideoStreamer::SendMessage(const string msg) {
    const char * ptr = msg.data();
    send_packet(ptr, msg.size());
}

bool VideoStreamer::open_socket() {
    _socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
    if (_socket_desc == -1)
        throw std::runtime_error("Cant create socket");
    _in_address.sin_family = AF_INET;
    _in_address.sin_addr.s_addr = INADDR_ANY;
    _in_address.sin_port = 52000;

    int bind_res = bind(_socket_desc, (struct sockaddr *)&_in_address, sizeof (_in_address));
    if (bind_res == -1)
        throw std::runtime_error("Cant bind socket");
}

size_t VideoStreamer::send_packet(const char *buf, size_t sz) {
    size_t res = sendto(_socket_desc, buf, sz, MSG_CONFIRM, (struct sockaddr *)&_dest_address, sizeof (_dest_address));
    return res;
}
