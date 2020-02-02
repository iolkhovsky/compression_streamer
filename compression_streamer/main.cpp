#include <iostream>

#include "video_receiver.h"
#include "video_source.h"
#include "video_streamer.h"
#include "paginator.h"


using namespace std;

template<typename Iterator>
void print_container(Iterator f, Iterator t) {
    cout << "[";
    for (auto it = f; it != t; it++) {
        cout << *it << ", ";
    }
    cout << endl;
}

void ServerRoutine() {
    VideoSource webcam(0);
    VideoStreamer streamer;

    streamer.SetDestination("127.0.0.1", 53500);
    streamer.Init();

    while(true) {
        Mat buffer;
        webcam >> buffer;
        buffer >> streamer;

        std::cout << "Frame rate: " << webcam.GetFps() << endl;
        std::cout << "Source traffic (Mb/s):  " << StreamStatistics::convert_traffic(webcam.GetTraffic(), StreamStatistics::TrafficConversion::Byte2MegaBit) << endl;
        std::cout << "Transmitter traffic (Mb/s): " << StreamStatistics::convert_traffic(streamer.GetTraffic(), StreamStatistics::TrafficConversion::Byte2MegaBit) << endl;
//        imshow("Src", buffer);
//        if (cv::waitKey(10) == 'q')
//            break;
    }
}

void ClientRoutine() {
    VideoReceiver rec;
    rec.SetAddress("127.0.0.1", 53500);
    rec.Init();
    rec.StartReceive();
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Client main thread" << endl;
    }
}

int main(int argc, char** argv) {
    stringstream ss;
    if (argc > 1) {
        for (size_t i = 1; i < argc; i++)
            ss << argv[i] << " ";

        string mode;
        ss >> mode;
        if (mode == "server")
            ServerRoutine();
        else if (mode == "client")
            ClientRoutine();
    } else {
        ClientRoutine();
    }
    return 0;
}
