import cv2
from ultralytics import YOLO

# Загружаем модель YOLOv8 (например, nano версия)
model = YOLO('yolov8n.pt')

# Загружаем изображение
image_path = 'image.jpg'
img = cv2.imread(image_path)

# Детектируем объекты
results = model(img)[0]  # Берём первый результат из списка
# results = model(img, classes=[0])[0]  # только класс "person"

# Получаем информацию о классах
class_names = model.names  # Словарь: {id: 'label'}
print(class_names)
# Рисуем bounding boxes
for box in results.boxes:
    x1, y1, x2, y2 = map(int, box.xyxy[0])  # координаты прямоугольника
    cls_id = int(box.cls[0])               # ID класса
    conf = float(box.conf[0])              # Доверие
    label = f"{class_names[cls_id]} {conf:.2f}"

    # Рисуем прямоугольник
    cv2.rectangle(img, (x1, y1), (x2, y2), (0, 255, 0), 2)

    # Рисуем подпись над bbox
    (text_width, text_height), _ = cv2.getTextSize(label, cv2.FONT_HERSHEY_SIMPLEX, 0.5, 1)
    cv2.rectangle(img, (x1, y1 - 20), (x1 + text_width, y1), (0, 255, 0), -1)
    cv2.putText(img, label, (x1, y1 - 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1)

# Показываем результат через OpenCV
cv2.imshow("YOLOv8 Detection", img)
cv2.waitKey(0)
cv2.destroyAllWindows()