#pragma once

#include <string>

#include <mqueue.h>

namespace ipc {

    constexpr size_t QueuePermission = 0644;
    constexpr int QueueMsgsCount = 10;
    constexpr int QueueMessageSize = 128;
    constexpr int QueueBufferSize = QueueMessageSize + 32;

    enum class Direction{
        transmitter = 0x0,
        receiver = 0x1
    };

    class PosixMQueueTx {
    public:
        PosixMQueueTx(std::string name);
        void send(std::string msg) const;
    private:
        mqd_t _desc;
        std::string _name;
    };

    class PosixMQueueRx {
    public:
        PosixMQueueRx(std::string name);
        std::string receive() const;
    private:
        mqd_t _desc;
        std::string _name;
    };

}
