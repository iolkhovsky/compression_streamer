#include <iostream>

#include "video_receiver.h"
#include "video_source.h"
#include "video_streamer.h"
#include "paginator.h"
#include "video_codec.h"

using namespace std;

template<typename Iterator>
void print_container(Iterator f, Iterator t) {
    cout << "[";
    for (auto it = f; it != t; it++) {
        cout << *it << ", ";
    }
    cout << endl;
}

void ServerRoutine(bool compression=true) {
    VideoSource webcam(0);
    VideoStreamer streamer;

    streamer.SetDestination("127.0.0.1", 53500);
    streamer.Init();
    streamer.SetCompression(compression);

    while(true) {
        Mat buffer;
        webcam >> buffer;
        buffer >> streamer;

//        VideoCodec codec;

//        imshow("Encoded/Decoded", codec.decode(codec.encode(buffer)));
//        if (cv::waitKey(10) == 'q')
//            break;


        std::cout << "Frame rate: " << webcam.GetFps() << endl;
        std::cout << "Source traffic (Mb/s):  " << StreamStatistics::convert_traffic(webcam.GetTraffic(), StreamStatistics::TrafficConversion::Byte2MegaBit) << endl;
        std::cout << "Transmitter traffic (Mb/s): " << StreamStatistics::convert_traffic(streamer.GetTraffic(), StreamStatistics::TrafficConversion::Byte2MegaBit) << endl;

    }
}

void ClientRoutine() {
    VideoReceiver receiver;
    receiver.SetAddress("127.0.0.1", 53500);
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
    stringstream ss;
    if (argc > 1) {
        for (size_t i = 1; i < argc; i++)
            ss << argv[i] << " ";

        string mode;
        ss >> mode;
        if (mode == "server") {
            bool compression = false;
            if (argc > 2)
                ss >> compression;
            ServerRoutine(compression);
        } else if (mode == "client")
            ClientRoutine();
    } else {
//        ServerRoutine(true);
        ClientRoutine();
    }
    return 0;
}
