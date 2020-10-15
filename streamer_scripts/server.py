from ipc_manager import IpcManager

import cv2


if __name__ == "__main__":
    cap = cv2.VideoCapture(0)
    ipc = IpcManager(shmem_name="/udp_streamer_shmem", sem_name="/udp_streamer", mq_name="/udp_streamer")
    face_cascade = \
        cv2.CascadeClassifier('/home/igor/opencv/opencv/data/haarcascades/haarcascade_frontalface_default.xml')
    while True:
        ret, frame = cap.read()
        cv2.imshow('Source', frame)
        try:
            processed = frame.copy()
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            faces = face_cascade.detectMultiScale(gray, 1.3, 5)
            for (x, y, w, h) in faces:
                cv2.rectangle(processed, (x, y), (x + w, y + h), (255, 0, 0), 2)
            cv2.imshow('Processed', processed)
            ipc.write_frame(processed)
        except:
            print("Processing exception")
            ipc.write_frame(frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    cap.release()
    cv2.destroyAllWindows()
