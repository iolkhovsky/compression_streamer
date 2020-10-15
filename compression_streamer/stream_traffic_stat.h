#pragma once

#include <queue>
#include <chrono>

using std::queue;
using std::chrono::duration;
using std::chrono::time_point;
using std::chrono::steady_clock;

namespace statistics {

    namespace default_settings {
        constexpr int average_period_usec = 1e6;
    }

    class TrafficStat {
        using timestamp = steady_clock::time_point;

        struct Event {
            double data_amount;
            timestamp stamp;
        };

    public:
        TrafficStat() = default;
        TrafficStat(double time_interval_sec);
        void AddTransaction(double bytes_cnt);
        double GetAverageTraffic();

    private:
        queue<Event> _events;
        int _stat_interval_usec = default_settings::average_period_usec;
        double _accumulator = 0.0;

        void add_event(double data_sz, timestamp stamp);
        void remove_oldest_event();
        void check_fifo();
        int usecs_to_tail() const;
    };

    namespace TrafficConversion {
        constexpr long int Byte2Bit = 8;
        constexpr double Bit2Byte = 1. / Byte2Bit;
        constexpr double Bit2Kilobit = 1. / 1024.;
        constexpr double Bit2Megabit = 1. / (1024. * 1024.);
        constexpr double Bit2Gigabit = 1. / (1024. * 1024. * 1024.);

        constexpr double Byte2MegaBit = Byte2Bit * Bit2Megabit;
    }

    double convert_traffic(double traffic, double mode);
}

