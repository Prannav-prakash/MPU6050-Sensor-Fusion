#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float ax_off, ay_off, az_off; t

void setup(void) {
  Serial.begin(115200);
  if (!mpu.begin()) { while (1); }

  Serial.println("DO NOT MOVE SENSOR - CALIBRATING...");
  
  float x_sum = 0, y_sum = 0, z_sum = 0;
  int numSamples = 200;

  for (int i = 0; i < numSamples; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    x_sum += a.acceleration.x;
    y_sum += a.acceleration.y;
    z_sum += a.acceleration.z;
    delay(10);
  }

  // Calculate Averages
  ax_off = x_sum / numSamples;
  ay_off = y_sum / numSamples;
  az_off = (z_sum / numSamples) - 9.81; 

  Serial.println("CALIBRATION COMPLETE");
  Serial.print("Offsets: "); Serial.print(ax_off); 
  Serial.print(", "); Serial.print(ay_off); 
  Serial.print(", "); Serial.println(az_off);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Apply the dynamic offsets
  float accX = a.acceleration.x - ax_off;
  float accY = a.acceleration.y - ay_off;
  float accZ = a.acceleration.z - az_off;

  // Calculate Angles
  float roll = atan2(accY, accZ) * 57.2958;
  float pitch = atan2(-accX, sqrt(accY * accY + accZ * accZ)) * 57.2958;

  Serial.print("Roll:"); Serial.print(roll);
  Serial.print(",");
  Serial.print("Pitch:"); Serial.println(pitch);

  delay(1000);
}
