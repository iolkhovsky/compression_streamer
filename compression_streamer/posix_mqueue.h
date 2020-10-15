#pragma once

#include <string>

#include <mqueue.h>

namespace ipc {

    constexpr int QueueMsgsCount = 10;
    constexpr int QueueMessageSize = 128;
    constexpr int QueueBufferSize = QueueMessageSize + 32;

    class PosixMQueue {
    public:
        PosixMQueue(std::string name, bool blocking = true);
        int send(std::string msg) const;
        std::string receive() const;
    private:
        mqd_t _desc;
        std::string _name;
        mq_attr _attr;
        mutable std::string _buffer;
    };
}
