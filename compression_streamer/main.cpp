#include <iostream>

#include "video_receiver.h"
#include "video_source.h"
#include "video_streamer.h"
#include "paginator.h"
#include "video_codec.h"
#include "configurator.h"
#include "ipc_manager.h"

using namespace std;
using namespace streamer;
using namespace ipc;


void ServerRoutine(const Configurator& configurator) {
    VideoSource webcam(configurator.GetWebcamId());
    VideoStreamer streamer;
    IpcManager ipc_manager(configurator.GetShmem(), configurator.GetSemaphore(), configurator.GetMQueue());
    config_videostreamer(streamer, configurator);
    streamer.Init();

    while(true) {
        Mat buffer;
        webcam >> buffer;
        buffer >> streamer;
        buffer >> ipc_manager;

        if (configurator.GetDebug()) {
            std::cout << "Frame rate: " << webcam.GetFps() << endl;
            std::cout << "Source traffic (Mb/s):  " << statistics::convert_traffic(webcam.GetTraffic(), statistics::TrafficConversion::Byte2MegaBit) << endl;
            std::cout << "Transmitter traffic (Mb/s): " << statistics::convert_traffic(streamer.GetTraffic(), statistics::TrafficConversion::Byte2MegaBit) << endl;
        }
    }
}

void ClientRoutine(const Configurator& configurator) {
    VideoReceiver receiver;
    IpcManager ipc_manager(configurator.GetShmem(), configurator.GetSemaphore(), configurator.GetMQueue());
    config_videoreceiver(receiver, configurator);
    receiver.Init();
    receiver.StartReceive();
    Mat rec_frame;
    while(true) {
        receiver >> rec_frame;
        rec_frame >> ipc_manager;
        if (configurator.GetDebug()) {
            std::cout << "Receiver traffic (Mb/s): " << statistics::convert_traffic(receiver.GetTraffic(), statistics::TrafficConversion::Byte2MegaBit) << endl;
        }
        imshow("Receive buffer", rec_frame);
        if (cv::waitKey(1) == 'q')
            break;
    }
}

int main(int argc, char** argv) {
    Configurator config(argc, argv);
    cout << config;
    if (config.GetMode() == Configurator::GlobalModes::server)
        ServerRoutine(config);
    else
        ClientRoutine(config);
    return 0;
}
