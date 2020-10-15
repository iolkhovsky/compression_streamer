#include "server.h"

#include "video_source.h"
#include "video_streamer.h"
#include "ipc_manager.h"

#include <iostream>

using namespace streamer;
using namespace ipc;
using namespace std;
using namespace statistics;

void run_server(const streamer::Configurator& configurator) {
    auto videosource = make_video_source(configurator);
    VideoStreamer streamer;
    std::optional<IpcManager> ipc_manager;
    if (configurator.GetSource() != Configurator::StreamSources::ipc)
        ipc_manager = std::move(IpcManager(configurator.GetShmem(), configurator.GetSemaphore(), configurator.GetMQueue()));
    config_videostreamer(streamer, configurator);
    streamer.Init();

    while(true) {
        Mat buffer;
        videosource >> buffer;
        if (buffer.empty())
            continue;
        buffer >> streamer;
        if (configurator.GetServerSaveFrame() && ipc_manager)
            buffer >> ipc_manager.value();

        if (configurator.GetDebug()) {
            static int frame_counter = 0;
            std::cout << frame_counter << ":" << "Frame rate: " << videosource->GetFps() << endl;
            std::cout << frame_counter << ":" << "Source traffic (Mb/s):  " <<
                         convert_traffic(videosource->GetTraffic(), TrafficConversion::Byte2MegaBit) << endl;
            std::cout << frame_counter << ":" << "Transmitter traffic (Mb/s): " <<
                         convert_traffic(streamer.GetTraffic(), TrafficConversion::Byte2MegaBit) << endl;
            frame_counter++;
        }
    }
}
