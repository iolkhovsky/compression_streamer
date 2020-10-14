#include "posix_mqueue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdexcept>

#include <iostream>

namespace ipc {
    PosixMQueue::PosixMQueue(std::string name) {
        try {
            _name = name;

            struct mq_attr attr;
            attr.mq_flags = 0;
            attr.mq_maxmsg = QueueMsgsCount;
            attr.mq_msgsize = QueueMessageSize;
            attr.mq_curmsgs = 0;
            _desc = mq_open(name.c_str(), O_RDWR | O_CREAT | O_NONBLOCK, S_IRWXU | S_IRWXG, &attr);

            if (_desc == -1)
                throw std::runtime_error("MQueue opening failed");
        } catch (std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
    }

    int PosixMQueue::send(std::string msg) const {
        if (_desc)
            return mq_send(_desc, msg.c_str(), msg.size() + 1, 0);
        else
            throw std::runtime_error("Try to send via posix mqueue while queue hasn't been opened properly");
    }

    std::string PosixMQueue::receive() const {
        if (_desc) {
            std::string buffer(QueueBufferSize, '0');
            ssize_t msg_sz = mq_receive(_desc, buffer.data(), buffer.size(), NULL);
            return {buffer.begin(), std::next(buffer.begin(), msg_sz)};
        } else
            throw std::runtime_error("Try to receive via posix mqueue while queue hasn't been opened properly");
    }
}
