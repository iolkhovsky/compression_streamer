#pragma once

#include <chrono>
#include <deque>

using std::deque;
using std::chrono::steady_clock;

namespace StreamStatistics {

class TimingStat {
    using timestamp = steady_clock::time_point;

public:
    TimingStat() = default;
    void PushEvent();
    double GetFps() const;
    double GetAverageFps() const;

private:
    deque<timestamp> _tstamps;

    void push_timestamp(timestamp t);
    static size_t duration_msecs(timestamp last, timestamp first);
};

}



