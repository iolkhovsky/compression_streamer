from posix_shmem import PosixShmem
from posix_mqueue import PosixMqueue
from posix_semaphore import PosixSemaphore

import cv2
import numpy as np

SHMEM_DEFAULT_SIZE = 2500 * 2000 * 3


class IpcManager:

    def __init__(self, shmem_name, sem_name, mq_name):
        self._shmem = PosixShmem(shmem_name, SHMEM_DEFAULT_SIZE)
        self._sem = PosixSemaphore(sem_name)
        self._mq = PosixMqueue(mq_name)

    def __str__(self):
        return "StreamerIpcManager"

    def __call__(self, *args, **kwargs):
        pass

    def write_frame(self, frame):
        assert type(frame) == np.ndarray
        pass

    def read_frame(self):
        pass
