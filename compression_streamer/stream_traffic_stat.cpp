#include "stream_traffic_stat.h"

namespace statistics {

TrafficStat::TrafficStat(double time_interval_sec)
    : _stat_interval_usec(static_cast<int>(time_interval_sec * 1e6)) {
}

void TrafficStat::AddTransaction(double bytes_cnt) {
    add_event(bytes_cnt, steady_clock::now());
}

double TrafficStat::GetAverageTraffic() {
    check_fifo();
    double interval_sec = _stat_interval_usec / 1000000.;
    return _accumulator / interval_sec;
}

void TrafficStat::add_event(double data_sz, timestamp stamp) {
    _events.push({data_sz, stamp});
    _accumulator += data_sz;
    check_fifo();
}

void TrafficStat::remove_oldest_event() {
    auto first = _events.front();
    _events.pop();
    _accumulator -= first.data_amount;
}

int TrafficStat::usecs_to_tail() const {
    timestamp now = steady_clock::now();
    auto to_tail = now - _events.front().stamp;
    return static_cast<size_t>(std::chrono::duration_cast<std::chrono::microseconds>(to_tail).count());;
}

void TrafficStat::check_fifo() {
    std::chrono::microseconds usec(_stat_interval_usec);
    if (_events.size()) {
        int totail = usecs_to_tail();
        while (_events.size() && (totail > _stat_interval_usec)) {
            remove_oldest_event();
            totail = usecs_to_tail();
        }
    }
}

double convert_traffic(double traffic, double mode) {
    return traffic * mode;
}

}
