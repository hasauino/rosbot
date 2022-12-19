import cv2
from time import sleep
import sys


cap = cv2.VideoCapture(sys.argv[1])
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 2560)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1440)
ret, frame = cap.read()
while True:
	sleep(0.5)
	if not ret:
		print("failed")
		continue
	print("worked!")
	cv2.imwrite('image.jpg', frame)
cap.release()
