# Kinderdorp PCB 🎨✨

An interactive educational PCB designed for children to learn electronics through hands-on experimentation with LEDs, sensors, and programming.

![Kinderdorp PCB Top](media/bottom.jpg)
![Kinderdorp PCB Bottom](media/top.jpg)

## 📋 Overview

The Kinderdorp PCB is an ESP32-C3 based learning platform featuring addressable RGB LEDs, user input buttons, sensors, and multiple power options. The board includes fun visual elements and is designed to be both educational and engaging for young learners.

## ✨ Features

### 🎯 Core Components
- **ESP32-C3-WROOM-02-N4** microcontroller (4MB Flash)
- **8x WS2812B** addressable RGB LEDs arranged in a creative pattern
- **2x Tactile buttons** for user interaction
- **Light sensor (LDR)** for ambient light detection
- **Buzzer** for audio feedback
- **USB-C connector** for power and programming

### 🔋 Power Options
- **USB-C** power input (5V)
- **Dual CR2032** coin cell battery holders
- **3x AAA** battery pack option
- **AMS1117-3.3V** voltage regulator
- Battery voltage monitoring circuit

### 🔌 Connectivity
- **8-pin header** for expansion and prototyping
- **Programming pins** for development
- **Test points** for debugging

## 🔧 Technical Specifications

| Component | Specification |
|-----------|---------------|
| **Microcontroller** | ESP32-C3-WROOM-02-N4 (4MB) |
| **Operating Voltage** | 3.3V |
| **Power Input** | 5V via USB-C or 3-6V battery |
| **LEDs** | 8x WS2812B RGB (5050 package) |
| **Buttons** | 2x DTSM-61R tactile switches |
| **Buzzer** | KSSGK4B16-20 |
| **Light Sensor** | LDR07 photoresistor |
| **PCB Dimensions** | ~100mm x 100mm |

## 📍 Pin Mapping

### GPIO Assignments
```
GPIO0  - Programming/Boot button
GPIO1  - UART TX (programming)
GPIO3  - UART RX (programming)  
GPIO4  - User Button 1
GPIO5  - RGB LED Data Out
GPIO6  - SDA (I2C)
GPIO7  - SCL (I2C)
GPIO8  - User Button 2
GPIO9  - Buzzer Control
GPIO10 - Light Sensor (ADC)
GPIO18 - Battery Voltage Monitor
GPIO19 - RGB LED Data In
```

### LED Chain Order
The 8 WS2812B LEDs are connected in series:
`LED1 → LED2 → LED3 → LED4 → LED5 → LED6 → LED7 → LED8`

## 🚀 Getting Started

### Prerequisites
- **Arduino IDE** or **PlatformIO**
- **ESP32 board package** installed
- **FastLED** or **Adafruit NeoPixel** library for LED control

### Hardware Setup
1. **Power the board** via USB-C or install batteries
2. **Connect** USE the breakout pads USB+ and USB- or UART to program the board
3. **Install** any additional components if using expansion header

### Basic Programming Example

```cpp
#include <FastLED.h>

#define LED_PIN 19
#define NUM_LEDS 8
#define BUTTON1_PIN 4
#define BUTTON2_PIN 8
#define BUZZER_PIN 9
#define LDR_PIN 10

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Rainbow effect
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(i * 32, 255, 255);
  }
  FastLED.show();
  delay(100);
}
```

## 🎓 Educational Applications

### Beginner Projects
- **LED Patterns** - Create colorful light sequences
- **Button Response** - Interactive games with button presses
- **Light Detection** - Automatic brightness control
- **Sound Effects** - Musical patterns with the buzzer

### Intermediate Projects
- **Simon Says Game** - Memory game using LEDs and buttons
- **Light Meter** - Display ambient light levels
- **Digital Pet** - Virtual pet with LED expressions
- **Alarm Clock** - Wake-up light with buzzer

### Advanced Projects
- **WiFi Connectivity** - IoT integration and remote control
- **Sensor Dashboard** - Environmental monitoring
- **Interactive Art** - Motion-responsive installations
- **Educational Games** - Programming challenges

## 🔋 Power Management

### Battery Life Optimization
- Use **deep sleep mode** when inactive
- **Dim LEDs** or turn off when not needed
- **Monitor battery voltage** (GPIO18) for low battery warnings

### Battery Voltage Monitoring
```cpp
float getBatteryVoltage() {
  int rawValue = analogRead(18);
  // Voltage divider: 6V→3V, 4.5V→2.25V, 3V→1.5V, 2V→1V
  return (rawValue / 4095.0) * 3.3 * 2.0;
}
```

## 🛠️ Assembly Notes

### Battery Installation
- **Option 1**: Install 2x CR2032 coin cells in BT1 and BT2
- **Option 2**: Connect 3x AAA battery pack to BATTPACK1
- **Do not** install both battery options simultaneously (should not be possible if PCB is asssembled correctly)

## 🎨 Design Features

The PCB includes playful design elements:
- **Artistic patterns** and decorative silkscreen
- **Child-friendly** component layout
- **Clear labeling** for educational purposes
- **Robust construction** for classroom use

## 📄 License

This project is released under the [MIT License](LICENSE). Feel free to use, modify, and distribute for educational purposes.

## 👨‍💻 Author

**Casper R. Tak**

---

*Kinderdorp PCB - Making electronics fun and accessible for young (and older) minds! 🌟*
