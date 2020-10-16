import numpy as np
import cv2
import time


class TestPatternGenerator:

    def __init__(self, sz=(320, 240), fps=10.0, play=True):
        self._xsz = sz[0]
        self._ysz = sz[1]
        self._frame_id = 0
        self._period_sec = 1. / fps
        self._play = play

    def restart(self):
        self._frame_id = 0

    def read(self):
        frame = np.zeros(shape=(self._ysz, self._xsz), dtype=np.uint8)
        gradient = list(range(self._xsz + self._ysz))
        for line_id in range(self._ysz):
            pivot = (line_id + self._frame_id) % self._xsz
            shifted_gradient = gradient[pivot:] + gradient[:pivot]
            frame[line_id] = np.asarray(shifted_gradient[:self._xsz], dtype=np.uint8)
        if self._play:
            self._frame_id += 1
        time.sleep(self._period_sec)
        return True, cv2.cvtColor(frame, cv2.COLOR_GRAY2BGR)


if __name__ == "__main__":
    gen = TestPatternGenerator()
    for i in range(10):
        cv2.imwrite(str(i)+".jpg", gen.read()[1])
