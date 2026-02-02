# MPU6050-Sensor-Fusion

# MPU6050 3D Orientation Tracker
Real-time roll/pitch/yaw calculation using complementary filter with gyro drift compensation. Outputs CSV data for Python visualization.

Key Features:
- Auto-calibration (500-sample bias removal)
- Deadzone filtering to eliminate gyro drift when stationary  
- Complementary filter (96% gyro / 4% accel) for stable tilt angles
- Yaw integration with 0-360° wrapping

  Hardware
- MCU: Arduino Uno/Nano/ESP32 (any I2C-capable board)
- Sensor: MPU6050
- Wiring:
  - VCC → 5V (or 3.3V for ESP32)
  - GND → GND
  - SCL → A5 (Uno/Nano) or GPIO 22 (ESP32)
  - SDA → A4 (Uno/Nano) or GPIO 21 (ESP32)
  - AD0 → GND (sets I2C address to 0x68)
 
 Installation
1. Install libraries via Arduino Library Manager:
   - `Adafruit MPU6050`
   - `Adafruit Unified Sensor`
   - `Wire` (built-in)

2. Upload final code on to the micro contorller and use VS code or any other text editor for the 3D visualization using python


 Calibration (MUST READ)
**The sensor must be completely stationary during the first 1-2 seconds after power-up!**

The code runs a 500-sample calibration loop (`delay(2)` × 500 = 1 second) to establish:
- Accel bias: Removes zero-g offset
- Gyro bias: Removes idle drift (critical for yaw stability)
- Gravity removal: Subtracts 9.81 m/s² from Z-axis

If you move the sensor during startup, your readings will be wrong until reset.

set Baud rate to 115200

if (abs(gyroZ) < 0.4) gyroZ = 0; removes random yaw movement 

 OUTPUT : roll,pitch,yaw
