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
    std::pair<Mat, double> rec_data;
    cv::Mat rec_frame;
    while(true) {
        receiver >> rec_data;
        rec_frame = std::move(rec_data.first);
        if (configurator.GetClientSaveFrame())
            rec_frame >> ipc_manager;
        if (configurator.GetDebug()) {
            static int frame_counter = 0;
            cout << frame_counter << ":" << "Receiver input traffic (Mb/s): " <<
                         convert_traffic(receiver.GetInputTraffic(), TrafficConversion::Byte2MegaBit) << endl;
            cout << frame_counter << ":" << "Receiver traffic (Mb/s): " <<
                         convert_traffic(receiver.GetOutputTraffic(), TrafficConversion::Byte2MegaBit) << endl;
            cout << frame_counter << ":" << "FPS " <<
                    receiver.GetFPS() << endl;
            cout << frame_counter << ":" << "Frame integrity (%) " <<
                    static_cast<int>(rec_data.second * 100.0) << endl;
            frame_counter++;
        }
        if (!rec_frame.empty())
            imshow("Receive buffer", rec_frame);
        if (cv::waitKey(1) == 'q')
            break;
    }
}
