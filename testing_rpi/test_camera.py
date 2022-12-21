import cv2
from time import sleep
import sys
import base64

cap = cv2.VideoCapture(sys.argv[1])
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 128)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 72)
ret, frame = cap.read()
frame = cv2.flip(frame, 0)
while True:
    sleep(0.5)
    if not ret:
        print("failed")
        continue
    print("worked!")
    cv2.imwrite('image.jpg', frame)
    _, buffer = cv2.imencode(".{}".format("jpg"), frame)
    img_as_text = base64.b64encode(buffer)
    data = f"data:image/jpg;base64,{img_as_text.decode('utf-8')}"
    break
cap.release()
