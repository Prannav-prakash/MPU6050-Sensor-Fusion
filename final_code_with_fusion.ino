#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float ax_bias, ay_bias, az_bias, gx_bias, gy_bias, gz_bias;
float roll = 0, pitch = 0, yaw = 0; 
unsigned long lastTime;

void setup() {
  Serial.begin(115200);
  if (!mpu.begin()) { while (1); }

  // CALIBRATION
  float ax_s=0, ay_s=0, az_s=0, gx_s=0, gy_s=0, gz_s=0;
  for (int i = 0; i < 500; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    ax_s += a.acceleration.x; ay_s += a.acceleration.y; az_s += a.acceleration.z;
    gx_s += g.gyro.x; gy_s += g.gyro.y; gz_s += g.gyro.z;
    delay(2);
  }
  ax_bias = ax_s/500; ay_bias = ay_s/500; az_bias = (az_s/500)-9.81;
  gx_bias = gx_s/500; gy_bias = gy_s/500; gz_bias = gz_s/500;
  
  lastTime = millis();
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float dt = (millis() - lastTime) / 1000.0;
  lastTime = millis();

  // 1. Correct Gyro
  float gyroX = (g.gyro.x - gx_bias) * 57.3;
  float gyroY = (g.gyro.y - gy_bias) * 57.3;
  float gyroZ = (g.gyro.z - gz_bias) * 57.3;

  // 2. THE DRIFT FIX (Deadzone)
  // This stops the slow "ghost" turning when the sensor is still
  if (abs(gyroZ) < 1.5) gyroZ = 0; 

  // 3. Accel Angles (Using your working convention)
  float accX = a.acceleration.x - ax_bias;
  float accY = a.acceleration.y - ay_bias;
  float accZ = a.acceleration.z - az_bias;
  
  float rollAcc = atan2(accY, accZ) * 57.3;
  float pitchAcc = atan2(-accX, sqrt(accY*accY + accZ*accZ)) * 57.3;

  // 4. Complementary Filter
  roll = 0.96 * (roll + gyroX * dt) + 0.04 * rollAcc;
  pitch = 0.96 * (pitch + gyroY * dt) + 0.04 * pitchAcc;
  
  // 5. Yaw Integration
  yaw = yaw + gyroZ * dt;

  // 6. Wrap Yaw
  if (yaw < 0) yaw += 360; 
  if (yaw >= 360) yaw -= 360;

  // 7. Output to your Python Script
  Serial.print(roll); Serial.print(",");
  Serial.print(pitch); Serial.print(",");
  Serial.println(yaw);

  delay(10);
}
