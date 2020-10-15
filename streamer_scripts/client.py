from ipc_manager import IpcManager

import cv2


if __name__ == "__main__":
    ipc = IpcManager(shmem_name="/udp_streamer_shmem", sem_name="udp_streamer", mq_name="/udp_streamer")
    while cv2.waitKey(10) != ord('q'):
        frame = ipc.read_frame()
        if frame is not None:
            cv2.imshow("IPC stream", frame)
