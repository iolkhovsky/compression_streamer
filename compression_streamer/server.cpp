#include "server.h"

#include "video_source.h"
#include "video_streamer.h"
#include "ipc_manager.h"

#include <iostream>

using namespace streamer;
using namespace ipc;
using namespace std;

void run_server(const streamer::Configurator& configurator) {
    auto videosource = make_video_source(configurator);
    VideoStreamer streamer;
    IpcManager ipc_manager(configurator.GetShmem(), configurator.GetSemaphore(), configurator.GetMQueue());
    config_videostreamer(streamer, configurator);
    streamer.Init();

    while(true) {
        Mat buffer;
        videosource >> buffer;
        if (buffer.empty())
            continue;
        buffer >> streamer;
        if (configurator.GetServerSaveFrame())
            buffer >> ipc_manager;

        if (configurator.GetDebug()) {
            std::cout << "Frame rate: " << videosource->GetFps() << endl;
            std::cout << "Source traffic (Mb/s):  " << statistics::convert_traffic(streamer.GetTraffic(),
                                                                                   statistics::TrafficConversion::Byte2MegaBit) << endl;
            std::cout << "Transmitter traffic (Mb/s): " << statistics::convert_traffic(streamer.GetTraffic(),
                                                                                       statistics::TrafficConversion::Byte2MegaBit) << endl;
        }
    }
}
