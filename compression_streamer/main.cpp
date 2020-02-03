#include <iostream>

#include "video_receiver.h"
#include "video_source.h"
#include "video_streamer.h"
#include "paginator.h"
#include "video_codec.h"
#include "configurator.h"

using namespace std;

template<typename Iterator>
void print_container(Iterator f, Iterator t) {
    cout << "[";
    for (auto it = f; it != t; it++) {
        cout << *it << ", ";
    }
    cout << endl;
}

void ServerRoutine(Configurator& conf) {
    VideoSource src;
    if (conf.GetSource() == Configurator::StreamSources::webcamera) {
        src.Open(conf.GetWebcamId());
    } else if (conf.GetSource() == Configurator::StreamSources::videofile) {
        src.Open(conf.GetVideoPath());
        src.EnableCycle();
    }

    VideoStreamer streamer;
    streamer.SetDestination(conf.GetIp(), conf.GetUdp());
    streamer.Init();
    streamer.SetCompression(conf.GetCompressionFlag());
    streamer.SetQuality(conf.GetCompressionQuality());

    while(true) {
        Mat buffer;
        src >> buffer;
        buffer >> streamer;

        std::cout << "Frame rate: " << src.GetFps() << endl;
        std::cout << "Source traffic (Mb/s):  " << StreamStatistics::convert_traffic(src.GetTraffic(), StreamStatistics::TrafficConversion::Byte2MegaBit) << endl;
        std::cout << "Transmitter traffic (Mb/s): " << StreamStatistics::convert_traffic(streamer.GetTraffic(), StreamStatistics::TrafficConversion::Byte2MegaBit) << endl;
    }
}

void ClientRoutine(Configurator& conf) {
    VideoReceiver receiver;
    receiver.SetAddress(conf.GetIp(), conf.GetUdp());
    receiver.Init();
    receiver.StartReceive();
    Mat rec_frame;
    while(true) {
        receiver >> rec_frame;
        std::cout << "Receiver traffic (Mb/s): " << StreamStatistics::convert_traffic(receiver.GetTraffic(), StreamStatistics::TrafficConversion::Byte2MegaBit) << endl;
        imshow("Receive buffer", rec_frame);
        if (cv::waitKey(1) == 'q')
            break;
    }
}

int main(int argc, char** argv) {
    Configurator config(argc, argv);

    if (config.GetMode() == Configurator::GlobalModes::server) {
        ServerRoutine(config);
    } else {
        ClientRoutine(config);
    }
    return 0;
}
