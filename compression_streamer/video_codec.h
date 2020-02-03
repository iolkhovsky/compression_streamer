#pragma once

#include <vector>
#include <cstddef>

#include <opencv2/opencv.hpp>

using std::vector;
using cv::Mat;
using cv::imencode;
using cv::imdecode;

class VideoCodec {
public:
    VideoCodec();
    vector<uint8_t> encode(const Mat& image);
    Mat decode(const vector<uint8_t> compressed);
    void set_quality(int q);
private:
    vector<uint8_t> _encoded_buf;
    Mat _src_image;
    int _quality;
};
