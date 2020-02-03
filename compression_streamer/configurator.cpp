#include "configurator.h"

#include <iostream>

#include <algorithm>


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


    for (size_t i = 0; i < argc; i++) {
        string buf = argv[i];
        if (buf.size() < 5)
            continue;
        string prefix(buf.begin(), buf.begin()+2);
        if (prefix != "--")
            continue;
        size_t sep_pos = buf.find("=");
        if (sep_pos >= buf.length())
            continue;
        string key(buf.begin()+2, buf.begin()+sep_pos);
        string val(buf.begin()+sep_pos+1, buf.end());
        _expressions[key] = val;
    }
}

Configurator::GlobalModes Configurator::GetMode() const {
    if (_expressions.count("mode")) {
        if (_expressions.at("mode") == "server")
            _mode = GlobalModes::server;
        else if (_expressions.at("mode") == "client")
            _mode = GlobalModes::client;
    }
    return _mode;
}

string Configurator::GetIp() const {
    if (_expressions.count("ip")) {
        _ip = _expressions.at("ip");
    }
    return _ip;
}

size_t Configurator::GetUdp() const {
    if (_expressions.count("udp")) {
        stringstream ss;
        ss << _expressions.at("udp");
        ss >> _ip;
    }
    return _udp;
}

Configurator::StreamSources Configurator::GetSource() const {
    if (_expressions.count("source")) {
        if (_expressions.at("source") == "webcam")
            _source = StreamSources::webcamera;
        else if (_expressions.at("source") == "video")
            _source = StreamSources::videofile;
    }
    return _source;
}

size_t Configurator::GetWebcamId() const {
    if (_expressions.count("webcam_id")) {
        stringstream ss;
        ss << _expressions.at("webcam_id");
        ss >> _webcam_id;
    }
    return _webcam_id;
}

string Configurator::GetVideoPath() const {
    if (_expressions.count("video")) {
        _video_path = _expressions.at("video");
    }
    return _video_path;
}

bool Configurator::GetCompressionFlag() const {
    if (_expressions.count("compression_en")) {
        if (_expressions.at("compression_en") == "true")
            _compression_en = true;
        else
            _compression_en = false;
    }
    return _compression_en;
}

int Configurator::GetCompressionQuality() const {
    if (_expressions.count("quality")) {
        stringstream ss;
        ss << _expressions.at("quality");
        ss >> _compression_coeff;
    }
    return _compression_coeff;
}

size_t Configurator::GetPacketSize() const {
    if (_expressions.count("pack_size")) {
        stringstream ss;
        ss << _expressions.at("pack_size");
        ss >> _packet_size;
    }
    return _packet_size;
}
