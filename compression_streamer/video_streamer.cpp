#include "video_streamer.h"
#include "paginator.h"

VideoStreamer::~VideoStreamer() {
    close(_socket_desc);
}

VideoStreamer::VideoStreamer(string ip_dest, size_t udp_dest)
    : _destination_ip(ip_dest), _destination_udp(udp_dest) {
}

void VideoStreamer::Init() {
    open_socket();
    update_destination();
}

void VideoStreamer::SetDestination(string ip_dest, size_t udp_dest) {
    _destination_ip = ip_dest;
    _destination_udp = udp_dest;
}

void VideoStreamer::update_destination() {
    _dest_address.sin_family = AF_INET;
    _dest_address.sin_addr.s_addr = htonl(convert_addr(_destination_ip));
    _dest_address.sin_port = _destination_udp;
}

uint32_t VideoStreamer::convert_addr(string ip) {
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

void VideoStreamer::SendFrame(const Mat& frame) {
    Protocol::FrameDesc frame_info;
    frame_info.payload = vector<uint8_t>(frame.data, frame.data + frame.total() * frame.elemSize());
    frame_info.img_sz_x = frame.cols;
    frame_info.img_sz_y = frame.rows;
    frame_info.pixel_size = frame.elemSize();
    auto packs = _protocol.make_packets(frame_info);

    for (auto &pack: packs) {
        const char* tx_buf = reinterpret_cast<const char *>(pack.data());
        send_packet(tx_buf, pack.size());
        _traffic.AddTransaction(pack.size());
    }
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

size_t VideoStreamer::send_packet(uint8_t *buf, size_t sz) {
    size_t res = sendto(_socket_desc, buf, sz, MSG_CONFIRM, (struct sockaddr *)&_dest_address, sizeof (_dest_address));
    return res;
}

size_t VideoStreamer::GetTraffic() {
    return _traffic.GetAverageTraffic();
}

void operator>>(const Mat& frame, VideoStreamer& streamer) {
    streamer.SendFrame(frame);
}
