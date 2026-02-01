#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

// Variables to store the calculated error offsets
float ax_error = 0;
float ay_error = 0;
float az_error = 0;

void setup(void) {
  Serial.begin(115200);
  
  // Initialize the sensor
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) { delay(10); }
  }

  Serial.println("--- ACCELEROMETER CALIBRATION ---");
  Serial.println("Place sensor perfectly FLAT on the table.");
  Serial.println("Calculating offsets in 3... 2... 1...");
  delay(2000); // Time to let go of the wires

  int numSamples = 500;
  for (int i = 0; i < numSamples; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Sum up the raw readings
    ax_error += a.acceleration.x;
    ay_error += a.acceleration.y;
    // We subtract 9.81 from Z because we expect it to be 1G while flat
    az_error += (a.acceleration.z - 9.81); 
    
    delay(3); // Small delay for stable sampling
  }

  // Calculate the average error
  ax_error /= numSamples;
  ay_error /= numSamples;
  az_error /= numSamples;

  Serial.println("Calibration Complete!");
  Serial.print("X Offset: "); Serial.println(ax_error);
  Serial.print("Y Offset: "); Serial.println(ay_error);
  Serial.print("Z Offset: "); Serial.println(az_error);
  delay(2000);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Apply the calibration offsets
  float cleanAX = a.acceleration.x - ax_error;
  float cleanAY = a.acceleration.y - ay_error;
  float cleanAZ = a.acceleration.z - az_error;

  // Print to Serial Plotter
  // X and Y should be 0.0, Z should be 9.81
  Serial.print(cleanAX);
  Serial.print(",");
  Serial.print(cleanAY);
  Serial.print(",");
  Serial.println(cleanAZ);

  delay(20);
}
