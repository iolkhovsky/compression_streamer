from ipc_manager import IpcManager

import cv2


if __name__ == "__main__":
    cap = cv2.VideoCapture(0)
    ipc = IpcManager(shmem_name="/udp_streamer_shmem", sem_name="/udp_streamer", mq_name="/udp_streamer")
    face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
    while True:
        ret, frame = cap.read()
        cv2.imshow('Source', frame)
        ipc.write_frame(frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    cap.release()
    cv2.destroyAllWindows()
