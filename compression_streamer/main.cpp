#include <iostream>

#include "video_source.h"
#include "video_streamer.h"

using namespace std;

int main(int argc, char** argv) {
    VideoSource webcam(0);
    VideoStreamer streamer;
    streamer.SetDestination("127.0.0.1", 53000);
    streamer.SendMessage("Hey \n");
    while(true) {
        Mat buffer;
        webcam >> buffer;

        std::cout << "Frame rate: " << webcam.GetFps() << endl;
        std::cout << "Traffic (Mb/s):  " << StreamStatistics::convert_traffic(webcam.GetTraffic(), StreamStatistics::TrafficConversion::Byte2MegaBit) << endl;

        imshow("Src", buffer);
        if (cv::waitKey(10) == 'q')
            break;
    }

    return 0;
}
