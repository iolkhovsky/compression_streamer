#include "posix_mqueue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdexcept>

namespace ipc {
    MQueueTx::MQueueTx(std::string name) {

        _name = name;

        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = QueueMsgsCount;
        attr.mq_msgsize = QueueMessageSize;
        attr.mq_curmsgs = 0;
        _desc = mq_open(name.c_str(), O_WRONLY | O_CREAT, QueuePermission, &attr);

        if (_desc == -1)
            throw std::runtime_error("MQueue opening failed");
    }

    void MQueueTx::send(std::string msg) {

        int res = mq_send(_desc, msg.c_str(), msg.size() + 1, 0);
        if (res == -1) {
            throw std::runtime_error("Error while sending mesage using mqueue");
        }
    }

    MQueueRx::MQueueRx(std::string name) {

        _name = name;

        struct mq_attr attr;
        attr.mq_flags = 0;
        attr.mq_maxmsg = QueueMsgsCount;
        attr.mq_msgsize = QueueMessageSize;
        attr.mq_curmsgs = 0;
        _desc = mq_open(name.c_str(), O_RDONLY);

        if (_desc == -1)
            throw std::runtime_error("MQueue opening failed");
    }

    std::string MQueueRx::receive() {
        std::string buffer('0', QueueBufferSize);
        ssize_t msg_sz = mq_receive(_desc, buffer.data(), buffer.size(), NULL);
        return {buffer.begin(), std::next(buffer.begin(), msg_sz)};
    }
}
