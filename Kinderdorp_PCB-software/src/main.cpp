#include <Arduino.h>  // Required for PlatformIO
#include <FastLED.h>

// Hardware Configuration - Updated for new PCB
const int BUZZER = 10;      // Buzzer pin (GPIO10)
#define RGB_PIN 3      // Data pin for LED strip (GPIO3)

#include "kinderdorp_song.h"  // Include after BUZZER definition
#define BUTTON1 4      // Button 1 pin (GPIO4) - change color/pattern
#define BUTTON2 5      // Button 2 pin (GPIO5) - play song
#define BATT_SENSE 0   // Battery voltage sensing pin (GPIO0)
#define LDR_PIN 2      // Light sensor pin (GPIO2)
#define NUM_LEDS 8     // Total number of LEDs (based on schematic)

// Battery voltage thresholds (after voltage divider: actual voltage = reading * 2)
#define BATT_MIN 1.8           // Minimum battery voltage
#define BATT_MAX 3.5           // Maximum battery voltage  
#define BATT_NO_DETECT 0.5     // Below this = no battery detected

// Brightness levels based on power source - matching Christmas lights
#define BRIGHTNESS_USB 128     // 50% brightness for USB power (128/255 = 50%)
#define BRIGHTNESS_BATTERY 60  // 25% brightness for battery power (60/255 = 25%)

// LDR Configuration
#define LDR_THRESHOLD 2000      // Light threshold (0-4095) - adjust based on testing
#define LDR_HYSTERESIS 200      // Prevents flickering by adding hysteresis
#define LDR_CHECK_INTERVAL 1000 // Check LDR every 1 second

// LED Array
CRGB leds[NUM_LEDS];

// Button Handling
bool button1State = HIGH;
bool lastButton1State = HIGH;
bool button2State = HIGH;
bool lastButton2State = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// Power Management
enum PowerSource {
  POWER_USB,
  POWER_BATTERY
};
PowerSource currentPowerSource = POWER_USB;
int currentBrightness = BRIGHTNESS_USB;
bool wifiEnabled = true;

// Light Sensor Management
bool ledsEnabledByLight = true;  // LEDs enabled by light sensor
unsigned long lastLDRCheck = 0;
int lastLDRReading = 0;

// Display Mode Enum
enum DisplayMode {
  STATIC_COLOR,
  RAINBOW_MODE,
  SNAKE_MODE,
  RANDOM_BLINK,
  CHASE_MODE,
  BREATHE_MODE,
  WAVE_MODE,
  SPARKLE_MODE,      // Twinkling effect
  FIREWORK_MODE,     // Exploding pattern
  METEOR_MODE,       // Shooting star effect
  CANDY_CANE_MODE,   // Rotating stripes
  FADE_RANDOM,       // Fading patterns
  RANDOM_SCATTER,    // Random red/green pattern
  OFF_MODE
};

DisplayMode currentMode = STATIC_COLOR;

// Color Options (Used for STATIC_COLOR mode)
int currentColorIndex = 0;
// Color definitions for LED patterns
CRGB colorOptions[] = {
  CRGB::Red,
  CRGB::Green,
  CRGB::Blue,
  CRGB::Purple,
  CRGB::Yellow,
  CRGB::Cyan,
  CRGB::White,
  CRGB::Orange,
  CRGB::Pink,
  CRGB::Magenta,
  CRGB::Lime,
  CRGB::Aqua,
  CRGB::Indigo,
  CRGB::Violet,
  CRGB::Gold,
  CRGB::Silver
};
#define NUM_COLORS (sizeof(colorOptions) / sizeof(colorOptions[0]))

// Pattern Variables
unsigned long lastPatternUpdate = 0;
unsigned long patternUpdateInterval = 50;

// Pattern-specific speed controls (adjusted for battery life)
const unsigned long RAINBOW_SPEED = 150;   // Slower to save power
const unsigned long SNAKE_SPEED = 200;     // Slower snake movement
const unsigned long CHASE_SPEED = 180;     // Slower chase pattern
const unsigned long WAVE_SPEED = 120;      // Slower wave
const unsigned long BREATHE_SPEED = 40;    // Slower breathe
const unsigned long SPARKLE_SPEED = 200;   // Sparkle timing
const unsigned long FIREWORK_SPEED = 400;  // Firework timing
const unsigned long METEOR_SPEED = 200;    // Meteor timing
const unsigned long CANDY_SPEED = 600;     // Candy cane timing
const unsigned long FADE_SPEED = 200;      // Fade timing
const unsigned long SCATTER_SPEED = 1000;  // Random scatter timing

// Snake Pattern Variables
uint8_t snakeHeadPos = 0;
const uint8_t snakeLength = 3;  // Shorter snake for 8 LEDs
uint8_t snakeHue = 0;

// Random Blink Variables
uint8_t randomLEDs[3] = {0};    // Fewer active LEDs for power saving
uint8_t randomHues[3] = {0};
unsigned long randomBlinkInterval = 600;
unsigned long lastRandomUpdate = 0;

// Chase Pattern Variables
uint8_t chasePos = 0;
uint8_t chaseHue = 0;

// Breathe Pattern Variables
uint8_t breatheBrightness = 0;
bool breatheIncreasing = true;
uint8_t breatheHue = 0;

// Wave Pattern Variables
uint8_t waveOffset = 0;
uint8_t waveHue = 0;

// Sparkle Pattern Variables
uint8_t sparklePositions[NUM_LEDS];
uint8_t sparkleBrightness[NUM_LEDS];
uint8_t sparkleColors[NUM_LEDS];

// Firework Pattern Variables
struct Firework {
    int8_t position;
    uint8_t phase;  // 0=launch, 1=explode, 2=fade
    uint8_t brightness;
    uint8_t color;
};
Firework currentFirework = {-1, 0, 0, 0};

// Meteor Pattern Variables
int8_t meteorPos = -1;
uint8_t meteorTail[3];
uint8_t meteorColor = 0;

// Candy Cane Pattern Variables
uint8_t candyOffset = 0;
uint8_t stripeWidth = 2;

// Fade pattern variables
uint8_t fadeProgress = 0;
CRGB currentFadeColors[NUM_LEDS];
CRGB targetFadeColors[NUM_LEDS];
bool needNewFadeTarget = true;

// Song State and Variables (implemented in kinderdorp_song.cpp)
// Variables are declared as extern in kinderdorp_song.h

// Battery monitoring
unsigned long lastBatteryCheck = 0;
const unsigned long batteryCheckInterval = 10000; // Check every 10 seconds

// Function prototypes
void checkPowerSource();
void checkLightSensor();
bool shouldShowLEDs();
void printPowerStatus();
void checkButtons();
void handleButton1Press();
void handleButton2Press();
void updatePatterns();
void updateRainbowPattern();
void updateSnakePattern();
void updateRandomBlinkPattern(unsigned long currentTime);
void updateChasePattern();
void updateBreathePattern();
void updateWavePattern();
void updateSparklePattern();
void updateFireworkPattern();
void updateMeteorPattern();
void updateCandyCanePattern();
void updateFadeRandom();
void updateRandomScatter();
void turnOffAllLEDs();
void updateDisplay();
void startSong();
void updateSong();
void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  
  // Initialize hardware
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BATT_SENSE, INPUT);
  pinMode(LDR_PIN, INPUT);
  
  // Check initial power source and light conditions
  checkPowerSource();
  checkLightSensor();
  
  // Initialize LEDs with appropriate brightness
  FastLED.addLeds<WS2812B, RGB_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(currentBrightness);
  
  // Initial state
  updateDisplay();
  
  Serial.println("Ready!");
  printPowerStatus();
}

void loop() {
  static unsigned long lastHeartbeat = 0;
  static unsigned long sessionUptime = 0;
  
  // Heartbeat message every 3 seconds with prettier formatting
  unsigned long currentMillis = millis();
  if (currentMillis - lastHeartbeat >= 3000) {
    sessionUptime += 3;
    lastHeartbeat = currentMillis;
    
    // Read sensor values
    int ldrReading = analogRead(LDR_PIN);
    int battReading = analogRead(BATT_SENSE);
    float battVoltage = (battReading / 4095.0) * 3.3 * 2; // Actual voltage after divider
    
    Serial.println("\n=== Kinderdorp Status ===");
    Serial.print("⏱️  Uptime: ");
    Serial.print(sessionUptime / 60);
    Serial.print("m ");
    Serial.print(sessionUptime % 60);
    Serial.println("s");
    
    Serial.print("🎨 Mode: ");
    switch(currentMode) {
      case STATIC_COLOR: 
        Serial.print("Static ");
        Serial.print(currentColorIndex < NUM_COLORS ? (currentColorIndex == 0 ? "Red" : currentColorIndex == 1 ? "Green" : "Blue") : "Unknown");
        break;
      case RAINBOW_MODE: Serial.print("Rainbow"); break;
      case SNAKE_MODE: Serial.print("Snake"); break;
      case RANDOM_BLINK: Serial.print("Random Blink"); break;
      case CHASE_MODE: Serial.print("Chase"); break;
      case BREATHE_MODE: Serial.print("Breathe"); break;
      case WAVE_MODE: Serial.print("Wave"); break;
      case SPARKLE_MODE: Serial.print("Sparkle"); break;
      case FIREWORK_MODE: Serial.print("Firework"); break;
      case METEOR_MODE: Serial.print("Meteor"); break;
      case CANDY_CANE_MODE: Serial.print("Candy Cane"); break;
      case FADE_RANDOM: Serial.print("Fade Random"); break;
      case RANDOM_SCATTER: Serial.print("Random Scatter"); break;
      case OFF_MODE: Serial.print("OFF"); break;
    }
    Serial.println();
    
    Serial.print("💡 LEDs: ");
    Serial.print(shouldShowLEDs() ? "ON" : "OFF");
    Serial.print(" (Brightness: ");
    Serial.print((currentBrightness * 100) / 255);
    Serial.println("%)");
    
    if (songState == PLAYING_SONG) {
      Serial.print("🎵 Playing: Step ");
      Serial.print(songStep);
      Serial.println("/139");
    } else {
      Serial.println("🎵 Song: Ready");
    }
    
    Serial.print("🌅 Light: ");
    Serial.print(ldrReading);
    Serial.print(" (");
    Serial.print(ldrReading < LDR_THRESHOLD ? "Dark" : "Bright");
    Serial.println(")");
    
    Serial.print("🔋 Power: ");
    Serial.print(currentPowerSource == POWER_USB ? "USB" : "Battery");
    Serial.print(" - ");
    Serial.print(battVoltage, 2);
    Serial.println("V");
    
    Serial.println("========================\n");
  }
  
  checkButtons();
  updateSong();
  
  // Check if LEDs should be shown based on light sensor and force enable status
  if (shouldShowLEDs()) {
    updatePatterns();
  } else {
    // Turn off LEDs when it's too bright (unless song is playing)
    if (songState == IDLE) {
      turnOffAllLEDs();
    } else {
      updatePatterns(); // Always show LEDs during song
    }
  }
  
  // Periodic monitoring
  if (millis() - lastBatteryCheck > batteryCheckInterval) {
    checkPowerSource();
    lastBatteryCheck = millis();
  }
  
  if (millis() - lastLDRCheck > LDR_CHECK_INTERVAL) {
    checkLightSensor();
    lastLDRCheck = millis();
  }
  
  FastLED.show();
}

void checkPowerSource() {
  // Read battery voltage (voltage divider gives us half the actual voltage)
  int reading = analogRead(BATT_SENSE);
  float voltage = (reading / 4095.0) * 3.3; // Convert to voltage
  
  PowerSource oldPowerSource = currentPowerSource;
  
  if (voltage < BATT_NO_DETECT) {
    // No battery detected - running on USB
    currentPowerSource = POWER_USB;
    currentBrightness = BRIGHTNESS_USB;
    wifiEnabled = true;
  } else if (voltage >= BATT_MIN && voltage <= BATT_MAX) {
    // Battery detected
    currentPowerSource = POWER_BATTERY;
    currentBrightness = BRIGHTNESS_BATTERY;
    wifiEnabled = false;
  } else {
    // Unknown voltage - assume battery power for safety
    currentPowerSource = POWER_BATTERY;
    currentBrightness = BRIGHTNESS_BATTERY;
    wifiEnabled = false;
  }
  
  // Update brightness if power source changed
  if (oldPowerSource != currentPowerSource) {
    FastLED.setBrightness(currentBrightness);
    printPowerStatus();
    
    // Disable WiFi/Bluetooth for battery operation
    if (!wifiEnabled) {
      // Add WiFi disable code here if needed
      Serial.println("WiFi disabled");
    }
  }
}

void checkLightSensor() {
  int ldrReading = analogRead(LDR_PIN);
  
  // Apply hysteresis to prevent flickering
  if (!ledsEnabledByLight && ldrReading < (LDR_THRESHOLD - LDR_HYSTERESIS)) {
    // It's getting dark enough - enable LEDs
    ledsEnabledByLight = true;
    Serial.println("LEDs ON");
  } else if (ledsEnabledByLight && ldrReading > (LDR_THRESHOLD + LDR_HYSTERESIS)) {
    // It's getting too bright - disable LEDs (unless force enabled)
    ledsEnabledByLight = false;
    Serial.println("LEDs OFF");
  }
  
  lastLDRReading = ldrReading;
}

bool shouldShowLEDs() {
  // Always show LEDs during song playback
  if (songState == PLAYING_SONG) {
    return true;
  }
  
  // Follow light sensor only
  return ledsEnabledByLight;
}

void printPowerStatus() {
  Serial.print("Power Source: ");
  switch (currentPowerSource) {
    case POWER_USB:
      Serial.print("USB");
      break;
    case POWER_BATTERY:
      Serial.print("Battery");
      break;
  }
  Serial.print(", Brightness: ");
  Serial.print((currentBrightness * 100) / 255);
  Serial.print("%, Light Level: ");
  Serial.print(lastLDRReading);
  Serial.print(", LEDs: ");
  Serial.println(shouldShowLEDs() ? "Enabled" : "Disabled");
}

void checkButtons() {
  bool reading1 = digitalRead(BUTTON1);
  bool reading2 = digitalRead(BUTTON2);

  // Debounce logic
  if (reading1 != lastButton1State || reading2 != lastButton2State) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Button 1 - Change mode/color
    if (reading1 != button1State) {
      button1State = reading1;
      if (button1State == LOW) {
        handleButton1Press();
      }
    }

    // Button 2 - Play song
    if (reading2 != button2State) {
      button2State = reading2;
      if (button2State == LOW) {
        handleButton2Press();
      }
    }
  }

  lastButton1State = reading1;
  lastButton2State = reading2;
}

void handleButton1Press() {
  // Enable LEDs for better visibility when adjusting
  
  if (currentMode == STATIC_COLOR) {
    currentColorIndex = (currentColorIndex + 1) % NUM_COLORS;
    if (currentColorIndex == NUM_COLORS - 1) {
      // After going through all colors, switch to first pattern mode
      currentMode = RAINBOW_MODE;
      Serial.println("RAINBOW");
    } else {
      Serial.println(currentColorIndex);
    }
  } else {
    // Switch to next pattern mode
    currentMode = (DisplayMode)((int)currentMode + 1);
    if (currentMode > OFF_MODE) {
      currentMode = STATIC_COLOR;
      currentColorIndex = 0;
      Serial.println("STATIC");
    } else {
      Serial.println((int)currentMode);
    }
  }
  updateDisplay();
}

void handleButton2Press() {
  // Songs always play regardless of light conditions
  if (songState == IDLE) {
    startSong();
  } else {
    stopSong();
  }
}

void updatePatterns() {
  unsigned long currentTime = millis();
  
  if (songState != IDLE) return; // Don't update patterns while playing song
  
  // Set update interval based on current mode
  switch (currentMode) {
    case RAINBOW_MODE:
      patternUpdateInterval = RAINBOW_SPEED;
      break;
    case SNAKE_MODE:
      patternUpdateInterval = SNAKE_SPEED;
      break;
    case CHASE_MODE:
      patternUpdateInterval = CHASE_SPEED;
      break;
    case WAVE_MODE:
      patternUpdateInterval = WAVE_SPEED;
      break;
    case BREATHE_MODE:
      patternUpdateInterval = BREATHE_SPEED;
      break;
    case SPARKLE_MODE:
      patternUpdateInterval = SPARKLE_SPEED;
      break;
    case FIREWORK_MODE:
      patternUpdateInterval = FIREWORK_SPEED;
      break;
    case METEOR_MODE:
      patternUpdateInterval = METEOR_SPEED;
      break;
    case CANDY_CANE_MODE:
      patternUpdateInterval = CANDY_SPEED;
      break;
    case FADE_RANDOM:
      patternUpdateInterval = FADE_SPEED;
      break;
    case RANDOM_SCATTER:
      patternUpdateInterval = SCATTER_SPEED;
      break;
    case RANDOM_BLINK:
      // Uses its own timing
      break;
    default:
      patternUpdateInterval = 50;
      break;
  }
  
  if (currentTime - lastPatternUpdate >= patternUpdateInterval) {
    lastPatternUpdate = currentTime;
    
    switch (currentMode) {
      case STATIC_COLOR:
        // No animation needed
        break;
      case RAINBOW_MODE:
        updateRainbowPattern();
        break;
      case SNAKE_MODE:
        updateSnakePattern();
        break;
      case RANDOM_BLINK:
        updateRandomBlinkPattern(currentTime);
        break;
      case CHASE_MODE:
        updateChasePattern();
        break;
      case BREATHE_MODE:
        updateBreathePattern();
        break;
      case WAVE_MODE:
        updateWavePattern();
        break;
      case SPARKLE_MODE:
        updateSparklePattern();
        break;
      case FIREWORK_MODE:
        updateFireworkPattern();
        break;
      case METEOR_MODE:
        updateMeteorPattern();
        break;
      case CANDY_CANE_MODE:
        updateCandyCanePattern();
        break;
      case FADE_RANDOM:
        updateFadeRandom();
        break;
      case RANDOM_SCATTER:
        updateRandomScatter();
        break;
      case OFF_MODE:
        turnOffAllLEDs();
        break;
    }
  }
}

void updateRainbowPattern() {
  breatheHue += 2; // Faster color transition
  for (int i = 0; i < NUM_LEDS; i++) {
    // Create a more vibrant rainbow with offset per LED
    int hue = (breatheHue + (i * 32)) % 255; // More spread between LEDs
    leds[i] = CHSV(hue, 255, 255);
  }
}

void updateSnakePattern() {
  turnOffAllLEDs();
  
  snakeHeadPos = (snakeHeadPos + 1) % NUM_LEDS;
  snakeHue += 3; // Faster color change
  
  for (int i = 0; i < snakeLength; i++) {
    int pos = (snakeHeadPos - i + NUM_LEDS) % NUM_LEDS;
    int brightness = 255 - (i * 200 / snakeLength); // Keep tail brighter
    // Use current color selection for snake
    leds[pos] = colorOptions[currentColorIndex];
    leds[pos].fadeToBlackBy(255 - brightness);
  }
}

void updateRandomBlinkPattern(unsigned long currentTime) {
  if (currentTime - lastRandomUpdate >= randomBlinkInterval) {
    lastRandomUpdate = currentTime;
    
    // Turn off old random LEDs with fade
    for (int i = 0; i < 3; i++) {
      if (randomLEDs[i] < NUM_LEDS) {
        leds[randomLEDs[i]].fadeToBlackBy(180);
      }
    }
    
    // Generate new random LEDs with random colors from our palette
    for (int i = 0; i < 3; i++) {
      randomLEDs[i] = random8(0, NUM_LEDS);
      leds[randomLEDs[i]] = colorOptions[random(NUM_COLORS)];
    }
  }
}

void updateChasePattern() {
  // Fade all LEDs instead of turning off completely
  fadeToBlackBy(leds, NUM_LEDS, 100);
  
  // Create a comet tail effect
  for (int i = 0; i < 3; i++) {
    int pos = (chasePos - i + NUM_LEDS) % NUM_LEDS;
    int brightness = 255 - (i * 80);
    leds[pos] = colorOptions[currentColorIndex];
    leds[pos].fadeToBlackBy(255 - brightness);
  }
  
  chasePos = (chasePos + 1) % NUM_LEDS;
}

void updateBreathePattern() {
  if (breatheIncreasing) {
    breatheBrightness += 4; // Smoother breathing
    if (breatheBrightness >= 255) {
      breatheIncreasing = false;
    }
  } else {
    breatheBrightness -= 4;
    if (breatheBrightness <= 20) { // Don't go completely black
      breatheIncreasing = true;
      // Cycle through all our colors
      currentColorIndex = (currentColorIndex + 1) % NUM_COLORS;
    }
  }
  
  // Use current selected color for breathing effect
  CRGB breatheColor = colorOptions[currentColorIndex];
  breatheColor.fadeToBlackBy(255 - breatheBrightness);
  fill_solid(leds, NUM_LEDS, breatheColor);
}

void updateWavePattern() {
  waveOffset += 15; // Faster wave
  
  for (int i = 0; i < NUM_LEDS; i++) {
    // Create multiple wave peaks
    uint8_t sinBrightness = sin8(waveOffset + (i * 64)); // More waves
    // Alternate colors across the wave
    CRGB waveColor = colorOptions[(i + (waveOffset / 50)) % NUM_COLORS];
    waveColor.fadeToBlackBy(255 - sinBrightness);
    leds[i] = waveColor;
  }
}

void turnOffAllLEDs() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void updateDisplay() {
  if (songState != IDLE) return; // Don't change display while playing song
  
  // Only update display if LEDs should be shown
  if (!shouldShowLEDs()) {
    turnOffAllLEDs();
    return;
  }
  
  turnOffAllLEDs();
  
  switch (currentMode) {
    case STATIC_COLOR:
      fill_solid(leds, NUM_LEDS, colorOptions[currentColorIndex]);
      break;
    case SNAKE_MODE:
      snakeHeadPos = 0;
      snakeHue = random8(0, 255);
      break;
    case RANDOM_BLINK:
      for (int i = 0; i < 3; i++) {
        randomLEDs[i] = random8(0, NUM_LEDS);
        randomHues[i] = random8(0, 255);
      }
      break;
    case CHASE_MODE:
      chasePos = 0;
      chaseHue = random8(0, 255);
      break;
    case BREATHE_MODE:
      breatheBrightness = 0;
      breatheIncreasing = true;
      breatheHue = random8(0, 255);
      break;
    case WAVE_MODE:
      waveOffset = 0;
      waveHue = random8(0, 255);
      break;
    default:
      break;
  }
}

// Song functionality with LED integration
void startSong() {
    turnOffAllLEDs();
    
    // Select color based on current mode
    if (currentMode != STATIC_COLOR) {
        currentColorIndex = random8(0, NUM_COLORS - 1); // Avoid black
    }
    
    initSong();  // Call the base implementation from kinderdorp_song.cpp
}

// Song update with LED-specific functionality
void updateSong() {
    if (songState != PLAYING_SONG) return;
    
    updateSongCore();  // Call the base implementation from kinderdorp_song.cpp
    
    // Spectacular LED effects during song
    float progress = (float)songStep / 139.0; // Updated total song steps
    unsigned long currentTime = millis();
    
    // Create different effects based on song progress
    if (progress < 0.25) {
        // First quarter: Progressive fill with rainbow
        int ledsToLight = (int)(progress * 4 * NUM_LEDS);
        if (ledsToLight > NUM_LEDS) ledsToLight = NUM_LEDS;
        
        for (int i = 0; i < NUM_LEDS; i++) {
            if (i < ledsToLight) {
                // Rainbow effect
                leds[i] = CHSV((i * 255 / NUM_LEDS + currentTime / 20) % 255, 255, 255);
            } else {
                leds[i] = CRGB::Black;
            }
        }
    } else if (progress < 0.5) {
        // Second quarter: Pulsing all colors
        int brightness = (sin((currentTime / 200.0) + songStep * 0.5) + 1) * 127.5;
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = colorOptions[(currentColorIndex + i) % NUM_COLORS];
            leds[i].fadeToBlackBy(255 - brightness);
        }
    } else if (progress < 0.75) {
        // Third quarter: Rotating rainbow chase
        for (int i = 0; i < NUM_LEDS; i++) {
            int hue = ((currentTime / 30 + i * 32) % 255);
            leds[i] = CHSV(hue, 255, 255);
        }
    } else {
        // Final quarter: Sparkle and finale effects
        static unsigned long lastSparkle = 0;
        if (currentTime - lastSparkle > 100) {
            // Clear all
            fill_solid(leds, NUM_LEDS, CRGB::Black);
            
            // Random sparkles with current color
            for (int i = 0; i < NUM_LEDS / 2; i++) {
                int pos = random(NUM_LEDS);
                leds[pos] = colorOptions[random(NUM_COLORS)];
            }
            lastSparkle = currentTime;
        }
        
        // On final notes, make everything bright white
        if (songStep >= 135) {
            fill_solid(leds, NUM_LEDS, CRGB::White);
        }
    }
    
    // Make sure to show the LED updates
    FastLED.show();
}

// New pattern functions copied from Christmas code
void updateRandomScatter() {
    for(int i = 0; i < NUM_LEDS; i++) {
        if(random8(4) == 0) { // 1/4 chance to change each LED
            leds[i] = colorOptions[random(NUM_COLORS)];
        }
    }
}

void updateFadeRandom() {
    if (needNewFadeTarget) {
        // Save current colors as start point
        for(int i = 0; i < NUM_LEDS; i++) {
            currentFadeColors[i] = leds[i];
        }
        
        // Generate new random target colors
        for(int i = 0; i < NUM_LEDS; i++) {
            targetFadeColors[i] = colorOptions[random(NUM_COLORS)];
        }
        needNewFadeTarget = false;
        fadeProgress = 0;
    }
    
    // Update fade progress
    fadeProgress += 4;
    
    // Apply fade between current and target colors
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = blend(currentFadeColors[i], targetFadeColors[i], fadeProgress);
    }
    
    // Check if fade is complete
    if(fadeProgress >= 255) {
        needNewFadeTarget = true;
    }
}

void updateSparklePattern() {
    // Start new sparkles
    if(random8(3) == 0) {  // 1/3 chance to start new sparkle
        uint8_t pos = random8(NUM_LEDS);
        if(sparkleBrightness[pos] == 0) {
            sparklePositions[pos] = pos;
            sparkleBrightness[pos] = 255;
            sparkleColors[pos] = random8(NUM_COLORS);
        }
    }
    
    // Update existing sparkles
    for(int i = 0; i < NUM_LEDS; i++) {
        if(sparkleBrightness[i] > 0) {
            leds[i] = colorOptions[sparkleColors[i]];
            leds[i].nscale8(sparkleBrightness[i]);
            sparkleBrightness[i] = (sparkleBrightness[i] * 3) >> 2; // Fade out
        } else {
            leds[i] = CRGB::Black;
        }
    }
}

void updateFireworkPattern() {
    // Clear all LEDs first
    turnOffAllLEDs();
    
    // Start new firework if none active
    if(currentFirework.position == -1) {
        currentFirework.position = 0;
        currentFirework.phase = 0;
        currentFirework.brightness = 255;
        currentFirework.color = random8(NUM_COLORS);
    }
    
    // Update firework
    switch(currentFirework.phase) {
        case 0: // Launch
            leds[currentFirework.position] = colorOptions[currentFirework.color];
            leds[currentFirework.position].nscale8(currentFirework.brightness);
            currentFirework.position++;
            if(currentFirework.position >= NUM_LEDS/2) {
                currentFirework.phase = 1;
            }
            break;
            
        case 1: // Explode
            for(int i = 0; i < NUM_LEDS; i++) {
                if(random8(2) == 0) {
                    leds[i] = colorOptions[currentFirework.color];
                    leds[i].nscale8(currentFirework.brightness);
                }
            }
            currentFirework.brightness = (currentFirework.brightness * 7) >> 3;
            if(currentFirework.brightness < 40) {
                currentFirework.phase = 2;
            }
            break;
            
        case 2: // Reset
            currentFirework.position = -1;
            break;
    }
}

void updateMeteorPattern() {
    // Fade all LEDs
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i].nscale8(192);
    }
    
    // Start new meteor if needed
    if(meteorPos == -1) {
        meteorPos = NUM_LEDS;
        meteorColor = random8(NUM_COLORS);
        for(int i = 0; i < 3; i++) {
            meteorTail[i] = 255 - (i * 64);  // Decreasing brightness tail
        }
    }
    
    // Update meteor position
    meteorPos--;
    
    // Draw meteor and tail
    for(int i = 0; i < 3; i++) {
        int pos = meteorPos + i;
        if(pos >= 0 && pos < NUM_LEDS) {
            leds[pos] = colorOptions[meteorColor];
            leds[pos].nscale8(meteorTail[i]);
        }
    }
    
    // Reset when off screen
    if(meteorPos < -3) {
        meteorPos = -1;
    }
}

void updateCandyCanePattern() {
    candyOffset = (candyOffset + 1) % (NUM_LEDS * 2);
    
    for(int i = 0; i < NUM_LEDS; i++) {
        bool useFirstColor = ((i + candyOffset/2) / stripeWidth) % 2 == 0;
        leds[i] = useFirstColor ? colorOptions[0] : colorOptions[1];
    }
}