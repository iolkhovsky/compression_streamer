#include "client.h"

#include "video_receiver.h"
#include "ipc_manager.h"
#include "configurator.h"

using namespace streamer;
using namespace ipc;

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
            std::cout << "Receiver traffic (Mb/s): " << statistics::convert_traffic(receiver.GetTraffic(),
                                                                                    statistics::TrafficConversion::Byte2MegaBit) << endl;
        }
        imshow("Receive buffer", rec_frame);
        if (cv::waitKey(1) == 'q')
            break;
    }
}
