#pragma once

#include <string>
#include <sstream>

using std::string;
using std::stringstream;

class Configurator {
    enum class GlobalModes {
        server,
        client
    };
    enum class StreamSources {
        webcamera,
        videofile
    };
public:
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
    GlobalModes _mode;
    StreamSources _source;
    string _ip;
    size_t _udp;
    bool _compression_en;
    int _compression_coeff;
    size_t _packet_size;
    string _video_path;
    size_t _webcam_id;
};
