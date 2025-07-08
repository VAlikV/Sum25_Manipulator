import numpy as np
import socket
from ultralytics import YOLO
import cv2

class UDPSender:
    def __init__(self, own_ip = "192.168.1.3", own_port = 8083):
        self.sock_ = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock_.bind((own_ip, own_port))
        self.sock_.settimeout(1.0)

    def array2str(self, arr):
        msg = "["
        for i in arr:
            msg = msg + str(i) + ","
        msg = msg[:-1] + "]"
        return msg
    
    def sendMessage(self, ip, port, array):
        msg = self.array2str(array)
        print(msg)
        b = msg.encode('utf-8')
        self.sock_.sendto(b, (ip, port))

class Control:
    def __init__(self, model_path):
        self.model_ = YOLO(model_path)
        self.class_names_ = self.model_.names

    def forward(self, image, position):
        new_position = position.copy()
        copy_image = image.copy()

        results = self.model_(image)[0]  # Берём первый результат из списка
        # results = model(img, classes=[0])[0]  # только класс "person"

        max_conf = 0
        bb_center = [0,0]
        detected = False

        # Рисуем bounding boxes
        for box in results.boxes:
            detected = True
            x1, y1, x2, y2 = map(int, box.xyxy[0])  # координаты прямоугольника
            cls_id = int(box.cls[0])               # ID класса
            conf = float(box.conf[0])              # Доверие
            label = f"{self.class_names_[cls_id]} {conf:.2f}"

            # Рисуем прямоугольник
            cv2.rectangle(copy_image, (x1, y1), (x2, y2), (0, 255, 0), 2)
            
            if conf > max_conf:
                max_conf = conf
                bb_center = [(x1+x2)//2, (y1+y2)//2]

            # Рисуем подпись над bbox
            (text_width, text_height), _ = cv2.getTextSize(label, cv2.FONT_HERSHEY_SIMPLEX, 0.5, 1)
            cv2.rectangle(copy_image, (x1, y1 - 20), (x1 + text_width, y1), (0, 255, 0), -1)
            cv2.putText(copy_image, label, (x1, y1 - 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1)
        
        if detected:
            new_position[0] += (bb_center[0] - image.shape[0])/5000
            new_position[1] += (bb_center[1] - image.shape[1])/5000
        
        return copy_image, new_position