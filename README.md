# RoboSoccer Robot Control

This project implements a WiFi-controlled robot soccer bot using an ESP32 microcontroller. The robot can be controlled remotely via UDP commands over a local WiFi network.

## Features

- WiFi connectivity using ESP32
- UDP-based remote control
- Differential drive with two DC motors
- Real-time movement commands (forward, backward, left, right, stop)

## Hardware Requirements

- ESP32 development board
- 2x DC motors with motor driver (e.g., L298N)
- Power supply for motors (appropriate voltage/current)
- Jumper wires for connections

### Pin Connections

#### Right Motor (Motor A)
- IN1: GPIO 19
- IN2: GPIO 21
- EN (PWM): GPIO 18

#### Left Motor (Motor B)
- IN1: GPIO 32
- IN2: GPIO 33
- EN (PWM): GPIO 25

## Software Setup

1. Install Arduino IDE
2. Install ESP32 board support in Arduino IDE
3. Open `Robbosoccer.ino` in Arduino IDE
4. Update WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI";
   const char* password = "YOUR_PASS";
   ```
5. Upload the code to your ESP32 board

## Usage

1. Power on the ESP32
2. The robot will connect to the specified WiFi network
3. Note the IP address printed in the Serial Monitor
4. Send UDP packets to the ESP32's IP on port 4210 with the following commands:

### Control Commands

- `F`: Move forward
- `B`: Move backward
- `L`: Turn left
- `R`: Turn right
- `S`: Stop

### Example Python Control Script

```python
import socket

UDP_IP = "ESP32_IP_ADDRESS"  # Replace with actual IP
UDP_PORT = 4210

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Send forward command
sock.sendto(b"F", (UDP_IP, UDP_PORT))

# Send stop command
sock.sendto(b"S", (UDP_IP, UDP_PORT))
```

## Code Structure

- `setup()`: Initializes WiFi, UDP, and motor pins
- `loop()`: Listens for UDP packets and executes movement commands
- Motor control functions: `setMotor()`, `stopMotor()`, `stopMotors()`
- Movement functions: `forward()`, `backward()`, `turnLeft()`, `turnRight()`

## Notes

- Adjust the `Speed` variable (default 150) to control motor speed (0-255)
- Ensure your WiFi network allows UDP communication
- The robot uses PWM for speed control on the enable pins

## Troubleshooting

- Check Serial Monitor for connection status and IP address
- Verify WiFi credentials and network connectivity
- Ensure motor driver is properly powered and connected
- Test motor directions and swap IN1/IN2 if needed for correct rotation