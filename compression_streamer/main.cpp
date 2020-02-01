#include <iostream>

#include "video_source.h"

using namespace std;

int main(int argc, char** argv) {
    VideoSource webcam(0);
    while(true) {
        Mat buffer;
        webcam >> buffer;

        std::cout << "FPS: " << webcam.GetFps() << endl;
//        std::cout << "Traffic: " << webcam.GetTraffic() << endl;

        imshow("Src", buffer);
        if (cv::waitKey(10) == 'q')
            break;
    }

    return 0;
}
