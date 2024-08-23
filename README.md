# Raspberry Pi Pico W Speedometer with OLED Display and Light Sensor

This project is a speedometer application for the Raspberry Pi Pico W, utilizing the MPU6050 accelerometer/gyroscope, an SSD1306 OLED display, and an LDR (Light Dependent Resistor) to adjust the screen brightness.

## Components Used

- **Raspberry Pi Pico W**
- **MPU6050**: 3-axis accelerometer and gyroscope.
- **SSD1306 OLED Display**: 128x64 pixels.
- **LDR (Light Dependent Resistor)**

## Circuit Diagram

- **MPU6050**: Connect to I2C pins of the Pico (SDA, SCL).
- **OLED Display**: Connect to the same I2C bus as the MPU6050.
- **LDR**: Connect one leg to the 3.3V pin and the other leg to the LDR_PIN (GPIO 26).

## Features

- **Speed Calculation**: The speed is calculated by integrating acceleration over time.
- **OLED Display**: Displays the current speed in mph and includes a visual speedometer.
- **Adaptive Brightness**: The OLED brightness is adjusted based on ambient light using the LDR.

## Usage

1. Upload the code to the Raspberry Pi Pico W using your preferred IDE (like the Arduino IDE).
2. Ensure the components are wired correctly.
3. Power up the circuit and monitor the serial output for speed data.
4. Observe the OLED display for speed readings and the adaptive brightness feature.

## Libraries Required

- `Adafruit_MPU6050`
- `Adafruit_Sensor`
- `Adafruit_GFX`
- `Adafruit_SSD1306`
- `Wire`

Install these libraries via the Arduino Library Manager.

## License

This project is open-source and available under the MIT License.
