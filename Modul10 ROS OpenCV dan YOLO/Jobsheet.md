# JOBSHEET MODUL 10: ROS 2 OPENCV DAN YOLO

**Program Studi:** Teknik Mekatronika dan Robotika  
**Mata Kuliah:** Praktikum Mekatronika dan Robotika  
**Modul:** 10 – ROS OpenCV dan YOLO  
**Pertemuan:** 19–20 (2 × 2 SKS)  
**Tanggal:** ___________________  
**Nama Kelompok:** ___________________  
**Anggota:**

| No | Nama | NIM |
|----|------|-----|
| 1  |      |     |
| 2  |      |     |
| 3  |      |     |
| 4  |      |     |

---

## A. TUJUAN PRAKTIKUM

1. Melakukan image processing dasar dengan OpenCV (filter, threshold, morphology)
2. Mendeteksi warna objek menggunakan HSV color space
3. Mendeteksi kontur dan menghitung centroid objek
4. Menggunakan cv_bridge untuk konversi ROS Image ↔ OpenCV
5. Membuat node ROS 2 untuk image processing
6. Mengimplementasikan object detection dengan YOLOv8
7. Mengintegrasikan deteksi objek dengan kontrol robot (visual servoing)

---

## B. ALAT DAN BAHAN

| No | Item | Keterangan |
|----|------|-----------|
| 1 | Laptop/PC Ubuntu 22.04 | RAM ≥ 8 GB, GPU opsional |
| 2 | ROS 2 Humble + Gazebo | Dari Modul 08 |
| 3 | OpenCV | `pip3 install opencv-python` |
| 4 | Ultralytics YOLOv8 | `pip3 install ultralytics` |
| 5 | cv_bridge | `sudo apt install ros-humble-cv-bridge` |
| 6 | Robot URDF + kamera | Dari Modul 08 (dengan camera plugin) |

---

## C. LANGKAH KERJA

---

### PERCOBAAN 1: OPENCV DASAR

**Estimasi waktu: 20 menit**

Buat script Python (non-ROS) untuk image processing:

```python
import cv2
import numpy as np

# Baca gambar
img = cv2.imread('test_image.jpg')
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# Filter
blur = cv2.GaussianBlur(gray, (5, 5), 0)

# Threshold
ret, thresh = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)

# Morphology
kernel = np.ones((5, 5), np.uint8)
clean = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)

# Tampilkan side-by-side
combined = np.hstack([gray, blur, thresh, clean])
cv2.imshow('Processing Steps', cv2.resize(combined, (1280, 240)))
cv2.waitKey(0)
```

**Dokumentasi:** Screenshot setiap tahap processing.

| Tahap | Deskripsi | Efek |
|-------|-----------|------|
| Original | | |
| Grayscale | | |
| Gaussian Blur | | |
| Otsu Threshold | | |
| Morphology Open | | |

**Checkpoint ✅:** Pipeline image processing berjalan.

---

### PERCOBAAN 2: COLOR DETECTION (HSV)

**Estimasi waktu: 25 menit**

```python
import cv2
import numpy as np

cap = cv2.VideoCapture(0)  # atau video file

def nothing(x): pass

# Trackbar untuk tuning HSV
cv2.namedWindow('Trackbar')
cv2.createTrackbar('H_min', 'Trackbar', 0, 180, nothing)
cv2.createTrackbar('H_max', 'Trackbar', 10, 180, nothing)
cv2.createTrackbar('S_min', 'Trackbar', 120, 255, nothing)
cv2.createTrackbar('S_max', 'Trackbar', 255, 255, nothing)
cv2.createTrackbar('V_min', 'Trackbar', 70, 255, nothing)
cv2.createTrackbar('V_max', 'Trackbar', 255, 255, nothing)

while True:
    ret, frame = cap.read()
    if not ret: break
    
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    h_min = cv2.getTrackbarPos('H_min', 'Trackbar')
    h_max = cv2.getTrackbarPos('H_max', 'Trackbar')
    s_min = cv2.getTrackbarPos('S_min', 'Trackbar')
    s_max = cv2.getTrackbarPos('S_max', 'Trackbar')
    v_min = cv2.getTrackbarPos('V_min', 'Trackbar')
    v_max = cv2.getTrackbarPos('V_max', 'Trackbar')
    
    lower = np.array([h_min, s_min, v_min])
    upper = np.array([h_max, s_max, v_max])
    mask = cv2.inRange(hsv, lower, upper)
    result = cv2.bitwise_and(frame, frame, mask=mask)
    
    cv2.imshow('Original', frame)
    cv2.imshow('Mask', mask)
    cv2.imshow('Result', result)
    
    if cv2.waitKey(1) & 0xFF == ord('q'): break
```

**Catat range HSV untuk mendeteksi 3 warna:**

| Warna | H Min | H Max | S Min | S Max | V Min | V Max |
|-------|-------|-------|-------|-------|-------|-------|
| Merah | | | | | | |
| Hijau | | | | | | |
| Biru | | | | | | |

**Checkpoint ✅:** 3 warna terdeteksi masing-masing.

---

### PERCOBAAN 3: DETEKSI KONTUR DAN CENTROID

**Estimasi waktu: 20 menit**

```python
def detect_objects(frame, mask):
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, 
                                    cv2.CHAIN_APPROX_SIMPLE)
    objects = []
    for cnt in contours:
        area = cv2.contourArea(cnt)
        if area > 500:
            x, y, w, h = cv2.boundingRect(cnt)
            cv2.rectangle(frame, (x,y), (x+w,y+h), (0,255,0), 2)
            
            M = cv2.moments(cnt)
            cx = int(M['m10'] / M['m00'])
            cy = int(M['m01'] / M['m00'])
            cv2.circle(frame, (cx, cy), 5, (0, 0, 255), -1)
            cv2.putText(frame, f'({cx},{cy}) A={area}', (x, y-10),
                       cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,0), 1)
            objects.append({'center': (cx, cy), 'area': area, 'bbox': (x,y,w,h)})
    
    return objects
```

**Catat hasil deteksi:**

| Objek | Centroid (cx, cy) | Area (pixels) | Bounding Box |
|-------|------------------|---------------|-------------|
| 1 | | | |
| 2 | | | |
| 3 | | | |

**Checkpoint ✅:** Kontur, centroid, dan bounding box terdeteksi.

---

### PERCOBAAN 4: ROS 2 IMAGE PROCESSING NODE

**Estimasi waktu: 30 menit**

Buat `image_processor.py`:

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
import numpy as np

class ImageProcessorNode(Node):
    def __init__(self):
        super().__init__('image_processor')
        self.sub = self.create_subscription(
            Image, '/camera/image_raw', self.callback, 10)
        self.pub_edges = self.create_publisher(Image, '/camera/edges', 10)
        self.pub_color = self.create_publisher(Image, '/camera/color_detect', 10)
        self.bridge = CvBridge()
        self.get_logger().info('Image processor started')
    
    def callback(self, msg):
        frame = self.bridge.imgmsg_to_cv2(msg, 'bgr8')
        
        # Edge detection
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        edges = cv2.Canny(gray, 50, 150)
        self.pub_edges.publish(self.bridge.cv2_to_imgmsg(edges, 'mono8'))
        
        # Color detection (red)
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, (0,120,70), (10,255,255))
        mask |= cv2.inRange(hsv, (170,120,70), (180,255,255))
        result = cv2.bitwise_and(frame, frame, mask=mask)
        self.pub_color.publish(self.bridge.cv2_to_imgmsg(result, 'bgr8'))
```

Jalankan + verifikasi di RViz2:
```bash
ros2 launch my_robot_description gazebo.launch.py   # Robot + kamera
ros2 run my_robot_pkg image_processor                 # Node processing
rviz2  # Add Image display: /camera/edges + /camera/color_detect
```

**Checkpoint ✅:** Edges dan color detection terlihat di RViz2.

---

### PERCOBAAN 5: YOLOV8 STANDALONE

**Estimasi waktu: 25 menit**

```python
from ultralytics import YOLO
import cv2

model = YOLO('yolov8n.pt')  # download otomatis

# Test pada gambar
results = model('test_image.jpg')
print(f'Detected {len(results[0].boxes)} objects')

for box in results[0].boxes:
    name = model.names[int(box.cls[0])]
    conf = box.conf[0].item()
    print(f'  {name}: {conf:.2f}')

# Simpan hasil
results[0].save(filename='result.jpg')
```

**Catat:**

| No | Objek Terdeteksi | Confidence | Bounding Box |
|----|-----------------|-----------|-------------|
| 1 | | | |
| 2 | | | |
| 3 | | | |

**Test pada webcam/video:**
```python
model = YOLO('yolov8n.pt')
results = model(source=0, show=True, conf=0.5)  # webcam
```

**Checkpoint ✅:** YOLOv8 mendeteksi objek dari gambar dan video.

---

### PERCOBAAN 6: YOLO + ROS 2

**Estimasi waktu: 30 menit**

```python
#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from std_msgs.msg import String
from cv_bridge import CvBridge
from ultralytics import YOLO
import json

class YoloNode(Node):
    def __init__(self):
        super().__init__('yolo_node')
        self.sub = self.create_subscription(
            Image, '/camera/image_raw', self.callback, 10)
        self.img_pub = self.create_publisher(Image, '/yolo/image', 10)
        self.det_pub = self.create_publisher(String, '/yolo/detections', 10)
        self.bridge = CvBridge()
        self.model = YOLO('yolov8n.pt')
        self.get_logger().info('YOLO node ready')
    
    def callback(self, msg):
        frame = self.bridge.imgmsg_to_cv2(msg, 'bgr8')
        results = self.model(frame, verbose=False, conf=0.5)
        
        detections = []
        for box in results[0].boxes:
            cls_name = self.model.names[int(box.cls[0])]
            conf = round(box.conf[0].item(), 2)
            bbox = [int(x) for x in box.xyxy[0].tolist()]
            detections.append({'class': cls_name, 'conf': conf, 'bbox': bbox})
        
        # Publish annotated image
        annotated = results[0].plot()
        self.img_pub.publish(self.bridge.cv2_to_imgmsg(annotated, 'bgr8'))
        
        # Publish detections
        det_msg = String()
        det_msg.data = json.dumps(detections)
        self.det_pub.publish(det_msg)
        
        if detections:
            self.get_logger().info(f'Detected: {[d["class"] for d in detections]}',
                                    throttle_duration_sec=1.0)
```

Verifikasi:
```bash
ros2 topic echo /yolo/detections
```

**Checkpoint ✅:** YOLO mendeteksi objek dari kamera Gazebo.

---

### PERCOBAAN 7: VISUAL SERVOING — FOLLOW RED OBJECT

**Estimasi waktu: 35 menit**

```python
class FollowRed(Node):
    def __init__(self):
        super().__init__('follow_red')
        self.img_sub = self.create_subscription(
            Image, '/camera/image_raw', self.img_cb, 10)
        self.cmd_pub = self.create_publisher(Twist, '/cmd_vel', 10)
        self.bridge = CvBridge()
        self.Kp_ang = 1.5
        self.Kp_lin = 0.001
    
    def img_cb(self, msg):
        frame = self.bridge.imgmsg_to_cv2(msg, 'bgr8')
        h, w = frame.shape[:2]
        
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, (0,120,70), (10,255,255))
        mask |= cv2.inRange(hsv, (170,120,70), (180,255,255))
        
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL,
                                         cv2.CHAIN_APPROX_SIMPLE)
        
        cmd = Twist()
        if contours:
            largest = max(contours, key=cv2.contourArea)
            area = cv2.contourArea(largest)
            if area > 300:
                M = cv2.moments(largest)
                cx = int(M['m10'] / M['m00'])
                
                error_x = (cx - w//2) / (w//2)  # -1 to 1
                cmd.angular.z = -self.Kp_ang * error_x
                
                target_area = 10000
                error_area = target_area - area
                cmd.linear.x = max(0, min(0.3, self.Kp_lin * error_area))
        
        self.cmd_pub.publish(cmd)
```

**Test:** Letakkan objek merah di Gazebo → robot mengikuti.

**Checkpoint ✅:** Robot mengikuti objek merah secara visual.

---

## D. ANALISA PERCOBAAN

### Analisa Percobaan 1 – Image Processing
1. Mengapa Gaussian blur dilakukan sebelum thresholding?
2. Apa perbedaan morphology opening vs closing?

### Analisa Percobaan 2 – Color Detection
1. Mengapa HSV lebih baik daripada BGR untuk color detection?
2. Apakah range HSV berubah di kondisi cahaya berbeda?

### Analisa Percobaan 3 – Kontur
1. Bagaimana menghitung jarak objek dari ukuran area kontur?
2. Apa kelemahan metode deteksi warna dibanding deep learning?

### Analisa Percobaan 4 – ROS 2 Image Node
1. Berapa FPS processing yang dicapai? Apa bottleneck-nya?
2. Bagaimana QoS mempengaruhi kualitas image transport?

### Analisa Percobaan 5-6 – YOLO
1. Bandingkan kecepatan inference YOLOv8n vs YOLOv8s.
2. Pada confidence berapa deteksi mulai reliable?

### Analisa Percobaan 7 – Visual Servoing
1. Apakah robot berhasil tracking smooth? Apa penyebab jitter?
2. Bagaimana menangani kasus objek terhalang sebagian (occlusion)?

---

## E. KESIMPULAN

1. ____________________________________________
2. ____________________________________________
3. ____________________________________________
4. ____________________________________________
5. ____________________________________________

---

## F. LAMPIRAN WAJIB

- [ ] Screenshot pipeline image processing
- [ ] Screenshot HSV color detection (3 warna)
- [ ] Screenshot YOLO detections
- [ ] Screenshot RViz2 annotated image
- [ ] Video visual servoing
- [ ] Kode program semua node

---

**Tanda Tangan Dosen/Asisten:** ___________________  
**Tanggal:** ___________________
