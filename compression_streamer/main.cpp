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
    vector<size_t> test = {1, 4, 10};
    using vecit = typename vector<size_t>::iterator;
    IteratorRange<vecit> range(test.begin(), test.end());
    Paginator<vecit> ptor(range, 0);

    cout << "Src array: ";
    print_container(test.begin(), test.end());

    cout << "Pages: " << endl;
    for (auto &page: ptor) {
        print_container(page.begin(), page.end());
    }
    cout << endl;




    VideoSource webcam(0);
    VideoStreamer streamer;
    streamer.SetDestination("127.0.0.1", 53000);
    streamer.Init();
    streamer.SendMessage("Hey \n");
    while(true) {
        Mat buffer;
        webcam >> buffer;

        std::cout << "Frame rate: " << webcam.GetFps() << endl;
        std::cout << "Traffic (Mb/s):  " << StreamStatistics::convert_traffic(webcam.GetTraffic(), StreamStatistics::TrafficConversion::Byte2MegaBit) << endl;

//        imshow("Src", buffer);
//        if (cv::waitKey(10) == 'q')
//            break;
    }

    return 0;
}
