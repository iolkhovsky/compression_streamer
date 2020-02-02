#include <iostream>

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

int main(int argc, char** argv) {
    VideoSource webcam(0);
    VideoStreamer streamer;

    streamer.SetDestination("127.0.0.1", 53000);
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

    return 0;
}
