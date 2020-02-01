#include "stream_traffic_stat.h"

StreamTrafficStat::StreamTrafficStat() {
    _stat_interval_usec = 1e6;
    _data_amount_acc = 0;
}

StreamTrafficStat::StreamTrafficStat(double time_interval_sec)
    : _stat_interval_usec(static_cast<size_t>(time_interval_sec * 1e6)),
      _data_amount_acc(0) {
}

void StreamTrafficStat::AddTransaction(size_t bytes_cnt) {
    add_event(bytes_cnt, steady_clock::now());
}

double StreamTrafficStat::GetAverageTraffic() {
    check_fifo();
    double interval_sec = _stat_interval_usec / 1000000.;
    return _data_amount_acc / interval_sec;
}

void StreamTrafficStat::add_event(size_t data_sz, timestamp stamp) {
    check_fifo();
    _events.push({data_sz, stamp});
    _data_amount_acc += data_sz;
}

void StreamTrafficStat::check_fifo() {
    timestamp now = steady_clock::now();
    std::chrono::microseconds usec(_stat_interval_usec);
    while (_events.size()) {
        if ((_events.back().stamp + std::chrono::microseconds(_stat_interval_usec) < now)) {
            size_t datasz = _events.back().data_amount;
            _events.pop();
            _data_amount_acc -= datasz;
        } else {
            break;
        }
    }
}
