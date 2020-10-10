#include "stream_traffic_stat.h"

namespace statistics {

TrafficStat::TrafficStat() {
    _stat_interval_usec = 1e6;
    _data_amount_acc = 0;
}

TrafficStat::TrafficStat(double time_interval_sec)
    : _stat_interval_usec(static_cast<size_t>(time_interval_sec * 1e6)),
      _data_amount_acc(0) {
}

void TrafficStat::AddTransaction(size_t bytes_cnt) {
    add_event(bytes_cnt, steady_clock::now());
}

double TrafficStat::GetAverageTraffic() {
    check_fifo();
    double interval_sec = _stat_interval_usec / 1000000.;
    return _data_amount_acc / interval_sec;
}

void TrafficStat::add_event(size_t data_sz, timestamp stamp) {
    _events.push({data_sz, stamp});
    _data_amount_acc += data_sz;
    check_fifo();
}

size_t TrafficStat::usecs_to_tail() const {
    timestamp now = steady_clock::now();
    auto to_tail = now - _events.front().stamp;
    size_t to_tail_us = static_cast<size_t>(std::chrono::duration_cast<std::chrono::microseconds>(to_tail).count());
    return to_tail_us;
}

void TrafficStat::check_fifo() {
    timestamp now = steady_clock::now();
    std::chrono::microseconds usec(_stat_interval_usec);
    if (_events.size()) {
        size_t totail = usecs_to_tail();
        while (_events.size() && (totail > _stat_interval_usec)) {
            size_t datasz = _events.back().data_amount;
            _events.pop();
            _data_amount_acc -= datasz;
            totail = usecs_to_tail();
        }
    }
}


double convert_traffic(double traffic, TrafficConversion mode) {
    double res = 0.;
    switch (mode) {
        case TrafficConversion::Byte2Bit: {
            res = traffic * 8;
        } break;
        case TrafficConversion::Byte2MegaBit: {
            res = traffic * 8 / (1024.*1024.);
        } break;
        case TrafficConversion::Byte2GigaBit: {
            res = traffic * 8 / (1024.*1024.*1024);
        } break;
        case TrafficConversion::Byte2MegaByte: {
            res = traffic / (1024.*1024.);
        } break;
        case TrafficConversion::Byte2GigaByte: {
            res = traffic / (1024.*1024.*1024.);
        } break;
    }
    return res;
}

}
