#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
// Create an MPU6050 object
Adafruit_MPU6050 mpu;


void setup() {
  // Start serial communication at 115200 baud rate
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");

  // Initialize I2C communication
  Wire.begin();

  // Initialize the MPU6050 sensor
  mpu.begin();
   // Calculates gyro offsets

  Serial1.println("MPU6050 initialized");
}

void loop() {
  // Update the MPU6050 sensor data
  mpu.update();

  // Print gyroscope data
  Serial1.print("GyroX: ");
  Serial1.print(mpu.getGyroX());
  Serial1.print("\tGyroY: ");
  Serial1.print(mpu.getGyroY());
  Serial1.print("\tGyroZ: ");
  Serial1.println(mpu.getGyroZ());

  delay(100); // Delay for 100 milliseconds
}
