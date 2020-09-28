#include "configurator.h"
#include "arg_parser.h"

#include <iostream>

namespace streamer {

    Configurator::Configurator(int argc, char** argv) {
        _mode = GlobalModes::client;
        _source = StreamSources::webcamera;
        _ip = "127.0.0.1";
        _udp = 53500;
        _compression_quality = 100;
        _packet_size = 1280;
        _video_path = "/home/igor/temp/2019-04-02-11-45-56-tv_11_tnf.avi";
        _webcam_id = 0;
        _debug = false;

        parser::ArgParser parser(argc, argv);
        auto mode = parser.read<std::string>("mode");
        if (mode)
            _mode = mode.value() == "server" ? GlobalModes::server : GlobalModes::client;
        auto source = parser.read<std::string>("source");
        if (source)
            _source = source.value() == "webcamera" ? StreamSources::webcamera : StreamSources::videofile;
        auto ip = parser.read<std::string>("ip");
        if (ip)
            _ip = ip.value();
        auto udp = parser.read<size_t>("port");
        if (udp)
            _udp = udp.value();
        auto compression = parser.read<int>("compression");
        if (compression)
            _compression_quality = compression.value();
        auto packet_size = parser.read<int>("packet_size");
        if (packet_size)
            _packet_size = packet_size.value();
        auto video_path = parser.read<std::string>("video_path");
        if (video_path)
            _video_path = video_path.value();
        auto cam_id = parser.read<int>("cam_id");
        if (cam_id)
            _webcam_id = cam_id.value();
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

    int Configurator::GetCompressionQuality() const {
        return _compression_quality;
    }

    size_t Configurator::GetPacketSize() const {
        return _packet_size;
    }

    bool Configurator::GetDebug() const {
        return _debug;
    }


    std::ostream& operator<<(std::ostream& os, const Configurator::GlobalModes& conf) {
        switch (conf) {
            case Configurator::GlobalModes::client: os << "client"; break;
            case Configurator::GlobalModes::server: os << "server"; break;
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Configurator::StreamSources& source) {
        switch (source) {
            case Configurator::StreamSources::videofile: os << "video file"; break;
            case Configurator::StreamSources::webcamera: os << "web camera"; break;
        }
        return os;
    }


    std::ostream& operator<<(std::ostream& os, const Configurator& conf) {
        std::cout << "---- Application settings ----" << std::endl;
        std::cout << "Mode: " << conf.GetMode() << std::endl;
        std::cout << "Debug: " << conf.GetDebug() << std::endl;
        std::cout << "IP: " << conf.GetIp() << std::endl;
        std::cout << "Port: " << conf.GetUdp() << std::endl;
        std::cout << "Video source: " << conf.GetSource() << std::endl;
        std::cout << "Web camera id: " << conf.GetWebcamId() << std::endl;
        std::cout << "Video file path: " << conf.GetVideoPath() << std::endl;
        std::cout << "Packet size: " << conf.GetPacketSize() << std::endl;
        std::cout << "Compression quality: " << conf.GetCompressionQuality() << std::endl;
        return os;
    }

}
