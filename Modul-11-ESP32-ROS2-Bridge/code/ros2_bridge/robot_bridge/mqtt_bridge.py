#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
import paho.mqtt.client as mqtt
import json
import time
import threading
from sensor_msgs.msg import Imu
from std_msgs.msg import Header, Float32

class MqttRos2Bridge(Node):
    """
    Bridge node that converts between MQTT and ROS2
    - Subscribe MQTT topics from ESP32 → Publish to ROS2
    - Subscribe ROS2 topics → Publish to MQTT (command to ESP32)
    """
    
    def __init__(self):
        super().__init__('mqtt_ros2_bridge')
        
        # Declare parameters
        self.declare_parameter('mqtt_host', 'localhost')
        self.declare_parameter('mqtt_port', 1883)
        self.declare_parameter('mqtt_keepalive', 60)
        
        mqtt_host = self.get_parameter('mqtt_host').value
        mqtt_port = self.get_parameter('mqtt_port').value
        mqtt_keepalive = self.get_parameter('mqtt_keepalive').value
        
        # ROS2 Publishers
        self.imu_pub = self.create_publisher(Imu, '/robot/sensor/imu', 10)
        self.motor1_pos_pub = self.create_publisher(Float32, '/robot/motor/1/position', 10)
        self.motor2_pos_pub = self.create_publisher(Float32, '/robot/motor/2/position', 10)
        
        # ACK tracking
        self.ack_received = set()
        self.ack_lock = threading.Lock()
        self.seq_counter = 0
        
        # Statistics
        self.stats = {
            'messages_received': 0,
            'messages_sent': 0,
            'ack_received': 0,
            'errors': 0
        }
        
        # Setup MQTT
        self.mqtt_client = mqtt.Client(client_id=f"ros2_bridge_{int(time.time())}")
        self.mqtt_client.on_connect = self.on_mqtt_connect
        self.mqtt_client.on_disconnect = self.on_mqtt_disconnect
        self.mqtt_client.on_message = self.on_mqtt_message
        
        # Connect to broker
        try:
            self.get_logger().info(f"Connecting to MQTT broker: {mqtt_host}:{mqtt_port}")
            self.mqtt_client.connect(mqtt_host, mqtt_port, mqtt_keepalive)
            self.mqtt_client.loop_start()
        except Exception as e:
            self.get_logger().error(f"MQTT connection failed: {e}")
        
        # Status timer
        self.create_timer(5.0, self.log_statistics)
    
    def on_mqtt_connect(self, client, userdata, flags, rc):
        """Callback when connected to MQTT broker"""
        if rc == 0:
            self.get_logger().info("✓ MQTT connected successfully")
            
            # Subscribe to topics
            topics = [
                ("robot/sensor/imu", 0),          # QoS 0 for sensor data
                ("robot/motor/1/position", 0),
                ("robot/motor/2/position", 0),
                ("robot/sensor/pressure", 0),
                ("robot/ack", 0)                  # ACK messages
            ]
            
            for topic, qos in topics:
                client.subscribe(topic, qos)
                self.get_logger().info(f"Subscribed: {topic} (QoS {qos})")
        else:
            self.get_logger().error(f"✗ MQTT connection failed with code {rc}")
    
    def on_mqtt_disconnect(self, client, userdata, rc):
        """Callback when disconnected from MQTT broker"""
        if rc != 0:
            self.get_logger().warn(f"MQTT disconnected unexpectedly (rc={rc})")
        else:
            self.get_logger().info("MQTT disconnected gracefully")
    
    def on_mqtt_message(self, client, userdata, msg):
        """Callback when message received from MQTT"""
        try:
            self.stats['messages_received'] += 1
            topic = msg.topic
            
            # Route based on topic
            if topic == "robot/sensor/imu":
                self.handle_sensor_imu(msg.payload)
            
            elif topic.startswith("robot/motor/") and topic.endswith("/position"):
                motor_id = int(topic.split('/')[2])
                self.handle_motor_position(motor_id, msg.payload)
            
            elif topic == "robot/ack":
                self.handle_ack(msg.payload)
            
            else:
                self.get_logger().debug(f"Unhandled topic: {topic}")
        
        except Exception as e:
            self.stats['errors'] += 1
            self.get_logger().error(f"Error processing message: {e}")
    
    def handle_sensor_imu(self, payload):
        """Handle IMU sensor data"""
        try:
            # Parse JSON
            data = json.loads(payload.decode())
            
            # Verify CRC if present
            if 'crc' in data:
                if not self.verify_crc(data):
                    self.get_logger().warn(f"CRC mismatch: seq={data.get('seq', 'N/A')}")
                    return
            
            # Create ROS2 IMU message
            imu_msg = Imu()
            imu_msg.header = Header()
            imu_msg.header.stamp = self.get_clock().now().to_msg()
            imu_msg.header.frame_id = 'imu_link'
            
            # Fill data
            sensor_data = data.get('data', {})
            imu_msg.linear_acceleration.x = sensor_data.get('accel_x', 0.0)
            imu_msg.linear_acceleration.y = sensor_data.get('accel_y', 0.0)
            imu_msg.linear_acceleration.z = sensor_data.get('accel_z', 0.0)
            imu_msg.angular_velocity.x = sensor_data.get('gyro_x', 0.0)
            imu_msg.angular_velocity.y = sensor_data.get('gyro_y', 0.0)
            imu_msg.angular_velocity.z = sensor_data.get('gyro_z', 0.0)
            
            # Publish to ROS2
            self.imu_pub.publish(imu_msg)
            
        except json.JSONDecodeError as e:
            self.stats['errors'] += 1
            self.get_logger().error(f"JSON decode error: {e}")
        except Exception as e:
            self.stats['errors'] += 1
            self.get_logger().error(f"IMU handling error: {e}")
    
    def handle_motor_position(self, motor_id, payload):
        """Handle motor position feedback"""
        try:
            position = float(payload.decode())
            
            # Create ROS2 message
            msg = Float32()
            msg.data = position
            
            # Publish to appropriate topic
            if motor_id == 1:
                self.motor1_pos_pub.publish(msg)
            elif motor_id == 2:
                self.motor2_pos_pub.publish(msg)
            
        except ValueError as e:
            self.stats['errors'] += 1
            self.get_logger().error(f"Motor position parse error: {e}")
    
    def handle_ack(self, payload):
        """Handle acknowledgment message"""
        try:
            data = json.loads(payload.decode())
            seq = data.get('seq', -1)
            status = data.get('status', 'unknown')
            
            # Mark as acknowledged
            with self.ack_lock:
                self.ack_received.add(seq)
                self.stats['ack_received'] += 1
            
            self.get_logger().info(f"ACK: seq={seq}, status={status}")
            
        except Exception as e:
            self.stats['errors'] += 1
            self.get_logger().error(f"ACK handling error: {e}")
    
    def verify_crc(self, data_dict):
        """Verify message CRC"""
        received_crc = data_dict.pop('crc', None)
        if received_crc is None:
            return True  # No CRC provided
        
        # Calculate CRC from message (excluding CRC field)
        message_str = json.dumps(data_dict, sort_keys=True)
        calculated_crc = self.calculate_crc_simple(message_str)
        
        # Restore CRC field
        data_dict['crc'] = received_crc
        
        return received_crc == calculated_crc
    
    def calculate_crc_simple(self, message):
        """Simple CRC calculation (matches firmware)"""
        crc = sum(ord(c) for c in message)
        return f"{crc:08x}"
    
    def send_command_with_retry(self, command_dict, max_retry=3, timeout=1.0):
        """Send command to ESP32 with ACK and retry"""
        for attempt in range(max_retry):
            # Assign sequence number
            seq = self.seq_counter
            self.seq_counter += 1
            command_dict['seq'] = seq
            
            # Send command
            message = json.dumps(command_dict)
            result = self.mqtt_client.publish("robot/command", message, qos=1)
            self.stats['messages_sent'] += 1
            
            if result.rc != mqtt.MQTT_ERR_SUCCESS:
                self.get_logger().error(f"Publish failed: {result.rc}")
                continue
            
            self.get_logger().info(f"Command sent: seq={seq}, attempt={attempt+1}")
            
            # Wait for ACK
            start_time = time.time()
            while time.time() - start_time < timeout:
                with self.ack_lock:
                    if seq in self.ack_received:
                        self.ack_received.remove(seq)
                        self.get_logger().info(f"✓ Command acknowledged: seq={seq}")
                        return True
                
                time.sleep(0.01)  # 10ms polling interval
            
            # Timeout
            self.get_logger().warn(f"✗ Timeout: seq={seq}, retry {attempt+1}/{max_retry}")
        
        self.get_logger().error(f"Command failed after {max_retry} retries")
        return False
    
    def log_statistics(self):
        """Log communication statistics"""
        self.get_logger().info(
            f"Stats - RX:{self.stats['messages_received']} "
            f"TX:{self.stats['messages_sent']} "
            f"ACK:{self.stats['ack_received']} "
            f"ERR:{self.stats['errors']}"
        )
    
    def destroy_node(self):
        """Cleanup on shutdown"""
        self.get_logger().info("Shutting down MQTT bridge...")
        self.mqtt_client.loop_stop()
        self.mqtt_client.disconnect()
        super().destroy_node()

def main(args=None):
    rclpy.init(args=args)
    
    node = MqttRos2Bridge()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
