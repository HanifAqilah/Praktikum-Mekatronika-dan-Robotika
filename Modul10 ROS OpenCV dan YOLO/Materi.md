# MATERI MODUL 10: ROS 2 OPENCV DAN YOLO

**Program Studi:** Teknologi Rekayasa Otomasi  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 10 – ROS OpenCV dan YOLO  
**Platform:** Ubuntu 22.04 + ROS 2 Humble + OpenCV + YOLOv8  
**Estimasi Waktu Belajar:** 6–8 Jam

---

## DAFTAR ISI

1. [Pendahuluan Computer Vision di Robotika](#1-pendahuluan-computer-vision-di-robotika)
2. [OpenCV Dasar](#2-opencv-dasar)
3. [Image Processing dengan OpenCV](#3-image-processing-dengan-opencv)
4. [Color Space dan Color Detection](#4-color-space-dan-color-detection)
5. [Deteksi Tepi dan Kontur](#5-deteksi-tepi-dan-kontur)
6. [cv_bridge: OpenCV ↔ ROS 2](#6-cv_bridge-opencv--ros-2)
7. [ROS 2 Image Transport](#7-ros-2-image-transport)
8. [Deep Learning Object Detection](#8-deep-learning-object-detection)
9. [YOLOv8 untuk Object Detection](#9-yolov8-untuk-object-detection)
10. [Integrasi YOLO dengan ROS 2](#10-integrasi-yolo-dengan-ros-2)
11. [Aplikasi: Visual Servoing Dasar](#11-aplikasi-visual-servoing-dasar)
12. [Referensi](#12-referensi)

---

## 1. PENDAHULUAN COMPUTER VISION DI ROBOTIKA

### 1.1 Kenapa Vision?

Computer vision memberikan robot kemampuan **persepsi visual** — informasi paling kaya tentang lingkungan:

| Sensor | Informasi |
|--------|-----------|
| LIDAR | Jarak 2D/3D (titik-titik) |
| Ultrasonik | Jarak 1 titik |
| **Kamera** | **Warna, bentuk, tekstur, teks, wajah, objek** |

### 1.2 Alur Computer Vision di ROS 2

```
[Kamera (Gazebo/USB)] → topic: /camera/image_raw
        ↓
[cv_bridge] → OpenCV Mat
        ↓
[Processing: filter, deteksi, YOLO]
        ↓
[Publish hasil] → topic: /detection_results
        ↓
[Kontrol robot] → topic: /cmd_vel
```

---

## 2. OPENCV DASAR

### 2.1 Instalasi

```bash
pip3 install opencv-python opencv-contrib-python
# atau
sudo apt install python3-opencv
```

### 2.2 Operasi Dasar

```python
import cv2
import numpy as np

# Baca gambar
img = cv2.imread('robot.jpg')
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# Resize
resized = cv2.resize(img, (320, 240))

# Crop
cropped = img[100:300, 50:250]

# Tulis teks
cv2.putText(img, 'Hello ROS', (10, 30), 
            cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

# Gambar rectangle
cv2.rectangle(img, (50, 50), (200, 200), (0, 0, 255), 2)

# Tampilkan
cv2.imshow('Image', img)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

### 2.3 Video/Webcam

```python
cap = cv2.VideoCapture(0)  # webcam
while True:
    ret, frame = cap.read()
    if not ret: break
    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    cv2.imshow('Video', gray)
    if cv2.waitKey(1) & 0xFF == ord('q'): break

cap.release()
cv2.destroyAllWindows()
```

---

## 3. IMAGE PROCESSING DENGAN OPENCV

### 3.1 Filtering

```python
# Gaussian Blur — smoothing
blur = cv2.GaussianBlur(img, (5, 5), 0)

# Median Blur — noise removal
median = cv2.medianBlur(img, 5)

# Bilateral — smooth tapi jaga edge
bilateral = cv2.bilateralFilter(img, 9, 75, 75)
```

### 3.2 Thresholding

```python
# Simple threshold
ret, thresh = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)

# Otsu's (adaptive)
ret, otsu = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)

# Adaptive threshold
adaptive = cv2.adaptiveThreshold(gray, 255, 
    cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
```

### 3.3 Morphological Operations

```python
kernel = np.ones((5, 5), np.uint8)

# Erode — mengecilkan objek putih
eroded = cv2.erode(thresh, kernel, iterations=1)

# Dilate — membesarkan objek putih
dilated = cv2.dilate(thresh, kernel, iterations=1)

# Opening — erode + dilate (remove noise)
opening = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)

# Closing — dilate + erode (fill gaps)
closing = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel)
```

---

## 4. COLOR SPACE DAN COLOR DETECTION

### 4.1 Color Spaces

| Space | Komponen | Kegunaan |
|-------|---------|---------|
| BGR | Blue, Green, Red | Default OpenCV |
| HSV | Hue, Saturation, Value | Color detection (robust terhadap cahaya) |
| Grayscale | Intensity | Edge detection, thresholding |
| LAB | Lightness, a, b | Color comparison |

### 4.2 HSV Color Detection

```python
# Konversi ke HSV
hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

# Range warna merah
lower_red = np.array([0, 120, 70])
upper_red = np.array([10, 255, 255])
mask1 = cv2.inRange(hsv, lower_red, upper_red)

lower_red2 = np.array([170, 120, 70])
upper_red2 = np.array([180, 255, 255])
mask2 = cv2.inRange(hsv, lower_red2, upper_red2)

mask = mask1 | mask2

# Terapkan mask
result = cv2.bitwise_and(frame, frame, mask=mask)
```

### 4.3 Tabel Range HSV Umum

| Warna | H Min | H Max | S Min | S Max | V Min | V Max |
|-------|-------|-------|-------|-------|-------|-------|
| Merah | 0/170 | 10/180 | 120 | 255 | 70 | 255 |
| Hijau | 35 | 85 | 50 | 255 | 50 | 255 |
| Biru | 100 | 130 | 50 | 255 | 50 | 255 |
| Kuning | 20 | 35 | 100 | 255 | 100 | 255 |

---

## 5. DETEKSI TEPI DAN KONTUR

### 5.1 Canny Edge Detection

```python
edges = cv2.Canny(gray, 50, 150)
```

### 5.2 Deteksi Kontur

```python
contours, hierarchy = cv2.findContours(
    mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

for cnt in contours:
    area = cv2.contourArea(cnt)
    if area > 500:  # filter noise
        x, y, w, h = cv2.boundingRect(cnt)
        cv2.rectangle(frame, (x,y), (x+w, y+h), (0,255,0), 2)
        
        # Centroid
        M = cv2.moments(cnt)
        if M['m00'] > 0:
            cx = int(M['m10'] / M['m00'])
            cy = int(M['m01'] / M['m00'])
            cv2.circle(frame, (cx, cy), 5, (0, 0, 255), -1)
```

---

## 6. CV_BRIDGE: OPENCV ↔ ROS 2

### 6.1 Instalasi

```bash
sudo apt install ros-humble-cv-bridge
```

### 6.2 Konversi ROS Image ↔ OpenCV

```python
from cv_bridge import CvBridge

bridge = CvBridge()

# ROS Image → OpenCV
def image_callback(msg):
    cv_image = bridge.imgmsg_to_cv2(msg, 'bgr8')
    # sekarang bisa pakai OpenCV
    gray = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)
    
    # OpenCV → ROS Image (untuk publish)
    ros_img = bridge.cv2_to_imgmsg(gray, 'mono8')
    publisher.publish(ros_img)
```

---

## 7. ROS 2 IMAGE TRANSPORT

### 7.1 Image Subscriber Node

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2

class ImageProcessor(Node):
    def __init__(self):
        super().__init__('image_processor')
        self.sub = self.create_subscription(
            Image, '/camera/image_raw', self.image_cb, 10)
        self.pub = self.create_publisher(Image, '/camera/processed', 10)
        self.bridge = CvBridge()
    
    def image_cb(self, msg):
        frame = self.bridge.imgmsg_to_cv2(msg, 'bgr8')
        
        # Processing
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        edges = cv2.Canny(gray, 50, 150)
        
        # Publish
        out_msg = self.bridge.cv2_to_imgmsg(edges, 'mono8')
        self.pub.publish(out_msg)

def main():
    rclpy.init()
    rclpy.spin(ImageProcessor())
```

---

## 8. DEEP LEARNING OBJECT DETECTION

### 8.1 Generasi YOLO

| Versi | Tahun | Keunggulan |
|-------|-------|-----------|
| YOLOv3 | 2018 | Darknet-53 backbone |
| YOLOv5 | 2020 | PyTorch, mudah dipakai |
| YOLOv7 | 2022 | SOTA kecepatan + akurasi |
| **YOLOv8** | **2023** | **Ultralytics, modular, termudah** |

### 8.2 Arsitektur YOLO

```
Input Image (640×640)
     ↓
[Backbone: CSPNet] → Feature extraction
     ↓
[Neck: FPN + PAN] → Multi-scale features
     ↓
[Head: Detect] → Bounding boxes + class probabilities
     ↓
[NMS] → Final detections
```

---

## 9. YOLOV8 UNTUK OBJECT DETECTION

### 9.1 Instalasi

```bash
pip3 install ultralytics
```

### 9.2 Inference Sederhana

```python
from ultralytics import YOLO

model = YOLO('yolov8n.pt')  # nano model (fastest)

results = model('image.jpg')

for r in results:
    for box in r.boxes:
        x1, y1, x2, y2 = box.xyxy[0].tolist()
        conf = box.conf[0].item()
        cls = int(box.cls[0].item())
        name = model.names[cls]
        print(f'{name}: {conf:.2f} at ({x1:.0f},{y1:.0f})-({x2:.0f},{y2:.0f})')
```

### 9.3 Inference pada Video/Webcam

```python
model = YOLO('yolov8n.pt')

cap = cv2.VideoCapture(0)
while True:
    ret, frame = cap.read()
    if not ret: break
    
    results = model(frame, verbose=False)
    annotated = results[0].plot()  # gambar bounding box
    
    cv2.imshow('YOLO', annotated)
    if cv2.waitKey(1) & 0xFF == ord('q'): break
```

### 9.4 Model Sizes

| Model | Size | mAP50-95 | Speed (ms) |
|-------|------|----------|-----------|
| YOLOv8n | 6.2 MB | 37.3 | 1.2 |
| YOLOv8s | 22.5 MB | 44.9 | 2.1 |
| YOLOv8m | 52.0 MB | 50.2 | 4.7 |
| YOLOv8l | 83.7 MB | 52.9 | 7.8 |
| YOLOv8x | 130.5 MB | 53.9 | 12.2 |

---

## 10. INTEGRASI YOLO DENGAN ROS 2

### 10.1 YOLO Detection Node

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from std_msgs.msg import String
from cv_bridge import CvBridge
from ultralytics import YOLO
import cv2
import json

class YoloDetector(Node):
    def __init__(self):
        super().__init__('yolo_detector')
        self.sub = self.create_subscription(
            Image, '/camera/image_raw', self.image_cb, 10)
        self.img_pub = self.create_publisher(Image, '/yolo/annotated', 10)
        self.det_pub = self.create_publisher(String, '/yolo/detections', 10)
        self.bridge = CvBridge()
        self.model = YOLO('yolov8n.pt')
        self.get_logger().info('YOLO detector ready')
    
    def image_cb(self, msg):
        frame = self.bridge.imgmsg_to_cv2(msg, 'bgr8')
        results = self.model(frame, verbose=False)
        
        detections = []
        for r in results:
            for box in r.boxes:
                x1, y1, x2, y2 = box.xyxy[0].tolist()
                conf = box.conf[0].item()
                cls = int(box.cls[0].item())
                name = self.model.names[cls]
                detections.append({
                    'class': name, 'confidence': round(conf, 2),
                    'bbox': [int(x1), int(y1), int(x2), int(y2)]
                })
        
        # Publish annotated image
        annotated = results[0].plot()
        self.img_pub.publish(self.bridge.cv2_to_imgmsg(annotated, 'bgr8'))
        
        # Publish detections as JSON
        det_msg = String()
        det_msg.data = json.dumps(detections)
        self.det_pub.publish(det_msg)
```

---

## 11. APLIKASI: VISUAL SERVOING DASAR

### 11.1 Konsep

Visual servoing: menggunakan **informasi visual kamera** untuk mengontrol gerakan robot.

```
Deteksi objek → hitung posisi objek di frame → 
  jika di kiri frame → robot belok kiri
  jika di kanan → belok kanan
  jika di tengah → maju
```

### 11.2 Implementasi: Follow Red Ball

```python
class FollowBall(Node):
    def __init__(self):
        super().__init__('follow_ball')
        self.img_sub = self.create_subscription(
            Image, '/camera/image_raw', self.image_cb, 10)
        self.cmd_pub = self.create_publisher(Twist, '/cmd_vel', 10)
        self.bridge = CvBridge()
    
    def image_cb(self, msg):
        frame = self.bridge.imgmsg_to_cv2(msg, 'bgr8')
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        
        # Deteksi warna merah
        mask = cv2.inRange(hsv, (0, 120, 70), (10, 255, 255))
        mask |= cv2.inRange(hsv, (170, 120, 70), (180, 255, 255))
        
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, 
                                        cv2.CHAIN_APPROX_SIMPLE)
        
        cmd = Twist()
        if contours:
            largest = max(contours, key=cv2.contourArea)
            area = cv2.contourArea(largest)
            if area > 500:
                M = cv2.moments(largest)
                cx = int(M['m10'] / M['m00'])
                
                # cx: 0-640, center = 320
                error = (cx - 320) / 320.0  # normalized -1 to 1
                cmd.angular.z = -error * 1.5
                
                # Jarak berdasarkan area
                if area < 5000:
                    cmd.linear.x = 0.3  # maju
                elif area > 20000:
                    cmd.linear.x = -0.1  # mundur
        
        self.cmd_pub.publish(cmd)
```

---

## 12. REFERENSI

1. Bradski, G. & Kaehler, A. (2008). *Learning OpenCV*. O'Reilly.
2. Ultralytics YOLOv8 Docs: https://docs.ultralytics.com
3. OpenCV Documentation: https://docs.opencv.org
4. ROS 2 cv_bridge: https://github.com/ros-perception/vision_opencv
5. Jocher, G. et al. (2023). YOLO by Ultralytics. https://github.com/ultralytics/ultralytics
6. Redmon, J. et al. (2016). *You Only Look Once: Unified, Real-Time Object Detection*. CVPR.

---

Dokumen ini menjadi acuan utama materi Modul 10 ROS 2 OpenCV dan YOLO.
