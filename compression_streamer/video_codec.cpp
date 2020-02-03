#include "video_codec.h"

VideoCodec::VideoCodec() {
    _quality = 95;
}

vector<uint8_t> VideoCodec::encode(const Mat& image) {
    _src_image = image;
    vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(_quality);

    imencode(".jpg", _src_image, _encoded_buf, compression_params);

    return _encoded_buf;
}

Mat VideoCodec::decode(const vector<uint8_t> compressed) {
    _encoded_buf = compressed;
    return imdecode(_encoded_buf, 1);
}

void VideoCodec::set_quality(int q) {
    _quality = q;
}
