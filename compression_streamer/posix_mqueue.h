#pragma once

#include <string>

#include <mqueue.h>

namespace ipc {

    constexpr size_t QueuePermission = 0660;
    constexpr int QueueMsgsCount = 10;
    constexpr int QueueMessageSize = 128;
    constexpr int QueueBufferSize = QueueMessageSize + 32;

    enum class Direction{
        transmitter = 0x0,
        receiver = 0x1
    };

    class MQueueTx {
    public:
        MQueueTx(std::string name);
        void send(std::string msg);
    private:
        mqd_t _desc;
        std::string _name;
    };

    class MQueueRx {
    public:
        MQueueRx(std::string name);
        std::string receive();
    private:
        mqd_t _desc;
        std::string _name;
    };

}
