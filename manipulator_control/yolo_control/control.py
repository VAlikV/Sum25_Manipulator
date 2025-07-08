from classes import UDPSender, Control
import numpy as np
import cv2

position = np.array([0.55, 0.0, 0.63, 1.0, 0.0, 0.0,
                                    0.0, -1.0, 0.0,
                                    0.0, 0.0, -1.0])

old_position = position.copy()
eps = np.array([0.00001, 0.00001, 0.00001])

udp = UDPSender("127.0.0.1", 8083)

udp.sendMessage("127.0.0.1", 8081, position)

detection = Control('yolov8n.pt')

# ==============================================================================
# ==============================================================================

cap = cv2.VideoCapture(0)  # 0 — основная камера, 1 — вторая камера

while cap.isOpened(): 

    ret, img = cap.read()  # Считываем кадр
    if not ret:
        break
    img = cv2.resize(img, (640, 480))

    bb_img, position = detection.forward(img, position)

    cv2.imshow("Image", img)
    cv2.imshow("Boxes", bb_img)
    
    if np.abs(position[0:3] - old_position[0:3]).any() >= eps.any():

        udp.sendMessage("127.0.0.1", 8081, position)
        old_position = position.copy()
    
    cv2.waitKey(1)

# ==============================================================================
# ==============================================================================

