#include "video_source.h"

Mat VideoSource::Read() {
    _cap >> _buffer;
    _timing.PushEvent();
    _traffic.AddTransaction(_buffer.total()*_buffer.elemSize());
    return _buffer;
}

double VideoSource::GetFps() const {
    return _timing.GetAverageFps();
}

double VideoSource::GetTraffic() {
    return _traffic.GetAverageTraffic();
}

void operator>>(VideoSource &src, Mat& dst) {
    dst = src.Read();
}


