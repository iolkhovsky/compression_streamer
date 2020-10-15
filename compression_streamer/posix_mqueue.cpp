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
    PosixMQueue::PosixMQueue(std::string name, bool blocking) {
        try {
            _name = name;

            _attr.mq_flags = 0;
            _attr.mq_maxmsg = QueueMsgsCount;
            _attr.mq_msgsize = QueueMessageSize;
            _attr.mq_curmsgs = 0;
            if (blocking)
                _desc = mq_open(name.c_str(), O_RDWR | O_CREAT, S_IRWXG, &_attr);
            else
                _desc = mq_open(name.c_str(), O_RDWR | O_CREAT | O_NONBLOCK, S_IRWXG, &_attr);
            if (_desc == -1)
                throw std::runtime_error("MQueue opening failed");
            mq_getattr(_desc, &_attr);
            _buffer.resize(_attr.mq_msgsize, '0');
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
            int msg_sz = mq_receive(_desc, _buffer.data(), _buffer.size(), NULL);
            if (msg_sz > 0)
                return {_buffer.begin(), std::next(_buffer.begin(), msg_sz)};
            else
                return {};
        } else
            throw std::runtime_error("Try to receive via posix mqueue while queue hasn't been opened properly");
    }
}
