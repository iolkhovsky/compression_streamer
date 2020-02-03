#include "stream_timing_stat.h"

namespace StreamStatistics {

void TimingStat::PushEvent() {
    push_timestamp(steady_clock::now());
}


size_t TimingStat::duration_msecs(timestamp last, timestamp first) {
    auto dur_msecs = std::chrono::duration_cast<std::chrono::milliseconds>(last - first);
    return static_cast<size_t>(dur_msecs.count());
}

double TimingStat::GetFps() const {
    if (_tstamps.size() > 1) {
        size_t msecs = duration_msecs(_tstamps.at(0), _tstamps.at(1));
        if (msecs) {
            return 1000.0 / msecs;
        } else {
            return 1000.0;
        }
    } else {
        return 0;
    }
}

double TimingStat::GetAverageFps() const {
    if (_tstamps.size() > 1) {
        size_t msecs = duration_msecs(_tstamps.front(), _tstamps.back());
        if (msecs) {
            return (_tstamps.size() - 1)  * 1000.0 / msecs;
        } else {
            return 1000.0;
        }
    } else {
        return 0;
    }
}

void TimingStat::push_timestamp(timestamp t) {
    _tstamps.push_front(t);
    while (_tstamps.size() > 25) {
        _tstamps.pop_back();
    }
}

}
