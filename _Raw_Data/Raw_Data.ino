#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup(void) {
  Serial.begin(115200);

  if (!mpu.begin()) {
    while (1) { yield(); }
  }

  // MANUAL RANGE SETTINGS
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);    
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);       
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);     
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Format: AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ
  // Accelerometer Data
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.print(a.acceleration.z);
  Serial.print(",");

  // Gyroscope Data
  Serial.print(g.gyro.x);
  Serial.print(",");
  Serial.print(g.gyro.y);
  Serial.print(",");
  Serial.println(g.gyro.z); 

  delay(100);
}
