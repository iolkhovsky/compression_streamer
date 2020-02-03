#include "configurator.h"

Configurator::Configurator(int argc, char** argv) {
    _mode = GlobalModes::client;
    _source = StreamSources::webcamera;
    _ip = "127.0.0.1";
    _udp = 53500;
    _compression_en = false;
    _compression_coeff = 100;
    _packet_size = 1280;
    _video_path = "/home/igor/temp/2019-04-02-11-45-56-tv_11_tnf.avi";
    _webcam_id = 0;

    stringstream ss;
    for (size_t i = 0; i < argc; i++) {
        if (i > 0)
            ss << " ";
        ss << argv[i];
    }

}

Configurator::GlobalModes Configurator::GetMode() const {
    return _mode;
}

string Configurator::GetIp() const {
    return _ip;
}

size_t Configurator::GetUdp() const {
    return _udp;
}

Configurator::StreamSources Configurator::GetSource() const {
    return _source;
}

size_t Configurator::GetWebcamId() const {
    return _webcam_id;
}

string Configurator::GetVideoPath() const {
    return _video_path;
}

bool Configurator::GetCompressionFlag() const {
    return _compression_en;
}

int Configurator::GetCompressionQuality() const {
    return _compression_coeff;
}

size_t Configurator::GetPacketSize() const {
    return _packet_size;
}
