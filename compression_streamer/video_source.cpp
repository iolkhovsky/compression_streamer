#include "video_source.h"

Mat VideoSource::Read() {
    _cap >> _buffer;
    _timing_stat.PushEvent();
    _traffic_stat.AddTransaction(_buffer.total()*_buffer.elemSize());
    return _buffer;
}

double VideoSource::GetFps() const {
    return _timing_stat.GetAverageFps();
}

double VideoSource::GetTraffic() {
    return _traffic_stat.GetAverageTraffic();
}

void operator>>(VideoSource &src, Mat& dst) {
    dst = src.Read();
}

