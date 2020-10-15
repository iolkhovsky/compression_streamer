#include "cv_video_source.h"

namespace streamer {

    Mat CvVideoSource::Read() {
        _cap >> _buffer;
        _timing.PushEvent();
        _traffic.AddTransaction(_buffer.total()*_buffer.elemSize());
        return _buffer;
    }

    double CvVideoSource::GetFps() const {
        return _timing.GetAverageFps();
    }

    double CvVideoSource::GetTraffic() {
        return _traffic.GetAverageTraffic();
    }
}

