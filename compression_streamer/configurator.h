#pragma once

#include <string>
#include <sstream>

using std::string;
using std::stringstream;

namespace streamer {

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
        int GetCompressionQuality() const;
        size_t GetPacketSize() const;
        bool GetDebug() const;
    private:
        GlobalModes _mode;
        StreamSources _source;
        string _ip;
        size_t _udp;
        int _compression_quality;
        size_t _packet_size;
        string _video_path;
        size_t _webcam_id;
        bool _debug;
    };

    std::ostream& operator<<(std::ostream& os, const Configurator& conf);
}
