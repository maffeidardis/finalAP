import cv2

cam = cv2.VideoCapture(0)
result, image = cam.read()
cv2.imwrite("webcam.jpg", image)
cam.release()
del(cam)