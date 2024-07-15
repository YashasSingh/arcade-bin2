#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Create an MPU6050 object
Adafruit_MPU6050 mpu;

// Create an OLED display object
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define the analog pin for the LDR sensor
#define LDR_PIN 26

sensors_event_t accel_event, gyro_event, temp_event;

void setup() {
  // Start serial communication at 115200 baud rate
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");

  // Initialize I2C communication
  Wire.begin();

  // Initialize the MPU6050 sensor
  while (!mpu.begin()) {
    Serial1.println("MPU6050 not connected!");
    delay(1000);
  }
  Serial1.println("MPU6050 initialized");

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial1.println(F("SSD1306 allocation failed"));
    for (;;) ; // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}

void loop() {
  // Update the MPU6050 sensor data
  mpu.getAccelerometerSensor()->getEvent(&accel_event);
  mpu.getGyroSensor()->getEvent(&gyro_event);
  mpu.getTemperatureSensor()->getEvent(&temp_event);

  // Read the LDR sensor value
  int ldrValue = analogRead(LDR_PIN);

  // Print sensor data to serial
  Serial1.print("[");
  Serial1.print(millis());
  Serial1.print("] X: ");
  Serial1.print(accel_event.acceleration.x);
  Serial1.print(", Y: ");
  Serial1.print(accel_event.acceleration.y);
  Serial1.print(", Z: ");
  Serial1.print(accel_event.acceleration.z);
  Serial1.print(" m/s^2, LDR: ");
  Serial1.println(ldrValue);

  // Display sensor data on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("MPU6050 Data:");
  display.print("Accel X: ");
  display.println(accel_event.acceleration.x);
  display.print("Accel Y: ");
  display.println(accel_event.acceleration.y);
  display.print("Accel Z: ");
  display.println(accel_event.acceleration.z);
  display.println();
  display.print("LDR: ");
  display.println(ldrValue);
  display.display();

  delay(500);
}
