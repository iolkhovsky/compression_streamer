import posix_ipc


MQ_MAX_MSGS = 10
MQ_SIZE = 128


class PosixMQueue:

    def __init__(self, name, blocking=True):
        flags = posix_ipc.O_RDWR + posix_ipc.O_CREAT
        if not blocking:
            flags += posix_ipc.O_NONBLOCK
        self._mq = posix_ipc.MessageQueue(name, flags=flags, mode=0o666, max_messages=MQ_MAX_MSGS,
                                          read=True, write=True)

    def send(self, msg):
        assert type(msg) == str
        self._mq.send(msg)

    def receive(self, timeout=None):
        try:
            res, priority = self._mq.receive(timeout)
        except posix_ipc.BusyError:
            return None
        return res

