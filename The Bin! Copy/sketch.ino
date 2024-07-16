#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define constants and objects
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define LDR_PIN 26

Adafruit_MPU6050 mpu;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

sensors_event_t accel_event, gyro_event, temp_event;

float speedX = 0, speedY = 0, speedZ = 0;
unsigned long lastTime;

void setup() {
    // Initialize serial communication
    Serial1.begin(115200);
    Serial1.println("Hello, Raspberry Pi Pico W!");

    // Initialize I2C and sensors
    Wire.begin();
    while (!mpu.begin()) {
        Serial1.println("MPU6050 not connected!");
        delay(1000);
    }
    Serial1.println("MPU6050 initialized");

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial1.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    display.display();
    delay(2000);
    display.clearDisplay();

    lastTime = millis();
}

void loop() {
    unsigned long currentTime = millis();
    float deltaTime = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;

    // Retrieve sensor data
    mpu.getAccelerometerSensor()->getEvent(&accel_event);
    mpu.getGyroSensor()->getEvent(&gyro_event);
    mpu.getTemperatureSensor()->getEvent(&temp_event);

    // Integrate acceleration to estimate speed
    speedX += accel_event.acceleration.x * deltaTime;
    speedY += accel_event.acceleration.y * deltaTime;
    speedZ += accel_event.acceleration.z * deltaTime;

    // Calculate speed in mph
    float speed_ms = sqrt(speedX * speedX + speedY * speedY + speedZ * speedZ);
    float speed_mph = speed_ms * 2.23694;

    // Read and map LDR value to brightness
    int ldrValue = analogRead(LDR_PIN);
    int brightness = map(ldrValue, 0, 1023, 0, 255);
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(brightness);

    // Output to serial monitor
    Serial1.print("[");
    Serial1.print(millis());
    Serial1.print("] Speed: ");
    Serial1.print(speed_mph);
    Serial1.print(" mph, LDR: ");
    Serial1.println(ldrValue);

    // Update OLED display
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Speedometer");
    display.setTextSize(2);
    display.print("Speed: ");
    display.print(speed_mph, 1);
    display.println(" mph");

    // Draw speedometer animation
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT - 10;
    int radius = 20;
    display.drawCircle(centerX, centerY, radius, SSD1306_WHITE);

    float angle = map(speed_mph, 0, 100, -90, 90);
    float radian = angle * PI / 180;
    int needleX = centerX + radius * cos(radian);
    int needleY = centerY - radius * sin(radian);
    display.drawLine(centerX, centerY, needleX, needleY, SSD1306_WHITE);

    display.display();
    delay(100);
}
