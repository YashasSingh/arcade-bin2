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

float speedX = 0, speedY = 0, speedZ = 0;
unsigned long lastTime;

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
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial1.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  lastTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0; // deltaTime in seconds
  lastTime = currentTime;

  // Update the MPU6050 sensor data
  mpu.getAccelerometerSensor()->getEvent(&accel_event);
  mpu.getGyroSensor()->getEvent(&gyro_event);
  mpu.getTemperatureSensor()->getEvent(&temp_event);

  // Estimate speed by integrating acceleration
  speedX += accel_event.acceleration.x * deltaTime;
  speedY += accel_event.acceleration.y * deltaTime;
  speedZ += accel_event.acceleration.z * deltaTime;

  // Calculate the resultant speed in m/s
  float speed_ms = sqrt(speedX * speedX + speedY * speedY + speedZ * speedZ);

  // Convert speed to mph (1 m/s = 2.23694 mph)
  float speed_mph = speed_ms * 2.23694;

  // Read the LDR sensor value
  int ldrValue = analogRead(LDR_PIN);

  // Adjust OLED brightness based on LDR value
  int brightness = map(ldrValue, 0, 1023, 0, 255); // Map LDR value to brightness
  display.ssd1306_command(SSD1306_SETCONTRAST); // Set contrast command
  display.ssd1306_command(brightness); // Set the brightness level

  // Print sensor data to serial
  Serial1.print("[");
  Serial1.print(millis());
  Serial1.print("] Speed: ");
  Serial1.print(speed_mph);
  Serial1.print(" mph, LDR: ");
  Serial1.println(ldrValue);

  // Display sensor data on OLED with animation
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Speedometer");
  display.setTextSize(2);
  display.print("Speed: ");
  display.print(speed_mph, 1); // Print speed with 1 decimal place
  display.println(" mph");

  // Draw a simple speedometer animation
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT - 10;
  int radius = 20;
  display.drawCircle(centerX, centerY, radius, SSD1306_WHITE);

  // Calculate the needle angle (map speed to angle, max speed 100 mph)
  float angle = map(speed_mph, 0, 100, -90, 90);
  float radian = angle * PI / 180;
  int needleX = centerX + radius * cos(radian);
  int needleY = centerY - radius * sin(radian);
  display.drawLine(centerX, centerY, needleX, needleY, SSD1306_WHITE);

  display.display();

  delay(100);
}
