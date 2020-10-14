#include "video_codec.h"

namespace streamer {

    vector<uint8_t> VideoCodec::encode(const Mat& image, int compression) {
        _src_image = image;
        vector<int> compression_params;
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(compression);

        imencode(".jpg", _src_image, _encoded_buf, compression_params);

        return _encoded_buf;
    }

    Mat VideoCodec::decode(const vector<uint8_t> compressed) {
        _encoded_buf = compressed;
        return imdecode(_encoded_buf, cv::IMREAD_COLOR);
    }

}
