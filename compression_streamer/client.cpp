#include "client.h"

#include "video_receiver.h"
#include "ipc_manager.h"
#include "configurator.h"

using namespace streamer;
using namespace ipc;
using namespace statistics;

void run_client(const streamer::Configurator& configurator) {
    VideoReceiver receiver;
    IpcManager ipc_manager(configurator.GetShmem(), configurator.GetSemaphore(), configurator.GetMQueue());
    config_videoreceiver(receiver, configurator);
    receiver.Init();
    receiver.StartReceive();
    Mat rec_frame;
    while(true) {
        receiver >> rec_frame;
        if (configurator.GetClientSaveFrame())
            rec_frame >> ipc_manager;
        if (configurator.GetDebug()) {
            static int frame_counter = 0;
            std::cout << ":" << frame_counter << "Receiver traffic (Mb/s): " <<
                         convert_traffic(receiver.GetTraffic(), TrafficConversion::Byte2MegaBit) << endl;
            frame_counter++;
        }
        if (!rec_frame.empty())
            imshow("Receive buffer", rec_frame);
        if (cv::waitKey(1) == 'q')
            break;
    }
}
