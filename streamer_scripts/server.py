from ipc_manager import IpcManager
from test_pattern_generator import TestPatternGenerator

import argparse
import cv2


def parse_cmd_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", type=str, default="test",
                        help="Source of videostream")
    parser.add_argument("--play", type=bool, default=True,
                        help="Play video")
    parser.add_argument("--show_stream", type=bool, default=True,
                        help="Show video stream")
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_cmd_args()
    if args.source == "webcam":
        cap = cv2.VideoCapture(0)
    elif args.source == "test":
        cap = TestPatternGenerator(play=args.play)
    else:
        raise RuntimeError("Invalid stream source type")
    print("Source: ", args.source)
    print("Play: ", args.play)
    print("Show stream: ", args.show_stream)

    ipc = IpcManager(shmem_name="/udp_streamer_shmem", sem_name="/udp_streamer", mq_name="/udp_streamer")
    face_cascade = \
        cv2.CascadeClassifier('/home/igor/opencv/opencv/data/haarcascades/haarcascade_frontalface_default.xml')
    while True:
        ret, frame = cap.read()
        if args.show_stream:
            cv2.imshow('Source', frame)
        try:
            processed = frame.copy()
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            faces = face_cascade.detectMultiScale(gray, 1.3, 5)
            for (x, y, w, h) in faces:
                cv2.rectangle(processed, (x, y), (x + w, y + h), (255, 0, 0), 2)
            if args.show_stream:
                cv2.imshow('Processed', processed)
            ipc.write_frame(processed)
        except:
            print("Processing exception")
            ipc.write_frame(frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    cap.release()
    if args.show_stream:
        cv2.destroyAllWindows()
