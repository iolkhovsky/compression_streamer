#pragma once

#include <queue>
#include <chrono>

using std::queue;
using std::chrono::duration;
using std::chrono::time_point;
using std::chrono::steady_clock;

class StreamTrafficStat {
    using timestamp = steady_clock::time_point;

    struct Event {
        size_t data_amount;
        timestamp stamp;
    };

public:
    StreamTrafficStat();
    StreamTrafficStat(double time_interval_sec);
    void AddTransaction(size_t bytes_cnt);
    double GetAverageTraffic();

private:
    mutable queue<Event> _events;
    size_t _stat_interval_usec;
    size_t _data_amount_acc;

    void add_event(size_t data_sz, timestamp stamp);
    void check_fifo();
};
