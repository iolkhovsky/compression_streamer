#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>

using std::string;
using std::stringstream;
using std::vector;
using std::map;

class Configurator {
public:
    enum class GlobalModes {
        server,
        client
    };
    enum class StreamSources {
        webcamera,
        videofile
    };
    Configurator(int argc, char** argv);
    // common
    GlobalModes GetMode() const;
    string GetIp() const;
    size_t GetUdp() const;
    // streamer
    StreamSources GetSource() const;
    size_t GetWebcamId() const;
    string GetVideoPath() const;
    bool GetCompressionFlag() const;
    int GetCompressionQuality() const;
    size_t GetPacketSize() const;
private:
    mutable GlobalModes _mode;
    mutable StreamSources _source;
    mutable string _ip;
    mutable size_t _udp;
    mutable bool _compression_en;
    mutable int _compression_coeff;
    mutable size_t _packet_size;
    mutable string _video_path;
    mutable size_t _webcam_id;

    map<string, string> _expressions;
};
