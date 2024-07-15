#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
// Create an MPU6050 object
Adafruit_MPU6050 mpu;

sensors_event_t accel_event, gyro_event, temp_event;
void setup() {
  // Start serial communication at 115200 baud rate
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");

  // Initialize I2C communication
  Wire.begin();

  // Initialize the MPU6050 sensor
  while (!mpu.begin()) {
    Serial.println("MPU6050 not connected!");
    delay(1000);
  }
  Serial1.println("MPU6050 initialized");
}


void loop() {
  // Update the MPU6050 sensor data
  mpu.getAccelerometerSensor()->getEvent(&accel_event);
  mpu.getGyroSensor()->getEvent(&gyro_event);
  mpu.getTemperatureSensor()->getEvent(&temp_event);

  Serial1.print("[");
  Serial1.print(millis());
  Serial1.print("] X: ");
  Serial1.print(accel_event.acceleration.x);
  Serial1.print(", Y: ");
  Serial1.print(accel_event.acceleration.y);
  Serial1.print(", Z: ");
  Serial1.print(accel_event.acceleration.z);
  Serial1.println(" m/s^2");
  delay(500);
}
