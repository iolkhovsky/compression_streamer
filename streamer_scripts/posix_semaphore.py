import posix_ipc


class PosixSemaphore:

    def __init__(self, name):
        self._sem = posix_ipc.Semaphore(name, mode=0o600)
        self._locked = False

    def lock(self, timeout=None):
        self._locked = True
        try:
            self._sem.acquire(timeout=timeout)
        except posix_ipc.BusyError:
            self._locked = False
        return self._locked

    def unlock(self):
        self._sem.release()
        self._locked = False

    def __del__(self):
        if self._locked:
            self._sem.release()
