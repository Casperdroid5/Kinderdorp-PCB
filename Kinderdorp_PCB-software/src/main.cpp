#include <Arduino.h>  // Required for PlatformIO
#include <FastLED.h>

// Hardware Configuration - Updated for new PCB
#define RGB_PIN 1      // Data pin for LED strip (GPIO1)
#define BUZZER 10      // Buzzer pin (GPIO10)
#define BUTTON1 4      // Button 1 pin (GPIO4) - change color/pattern
#define BUTTON2 5      // Button 2 pin (GPIO5) - play song
#define BATT_SENSE 3   // Battery voltage sensing pin (GPIO3)
#define LDR_PIN 6      // Light sensor pin (GPIO6)
#define NUM_LEDS 8     // Total number of LEDs (based on schematic)

// Battery voltage thresholds (after voltage divider: actual voltage = reading * 2)
#define BATT_CR2032_MIN 2.75    // 5.5V / 2 = 2.75V (CR2032 batteries)
#define BATT_CR2032_MAX 3.5     // 7V / 2 = 3.5V (CR2032 batteries)
#define BATT_AAA_MIN 1.8        // 3.6V / 2 = 1.8V (AAA batteries)
#define BATT_AAA_MAX 2.75       // 5.5V / 2 = 2.75V (AAA batteries)
#define BATT_NO_DETECT 0.5      // Below this = no battery detected

// Brightness levels based on power source
#define BRIGHTNESS_USB 230      // 90% brightness for USB power
#define BRIGHTNESS_BATTERY 89   // 35% brightness for battery power

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
  POWER_CR2032,
  POWER_AAA
};
PowerSource currentPowerSource = POWER_USB;
int currentBrightness = BRIGHTNESS_USB;
bool wifiEnabled = true;

// Light Sensor Management
bool ledsEnabledByLight = true;  // LEDs enabled by light sensor
bool ledsForceEnabled = false;   // Override for button presses/songs
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
  OFF_MODE
};

DisplayMode currentMode = STATIC_COLOR;

// Color Options (Used for STATIC_COLOR mode)
int currentColorIndex = 0;
CRGB colorOptions[] = {
  CRGB::Red,
  CRGB::Green,
  CRGB::Blue,
  CRGB::Purple,
  CRGB::Yellow,
  CRGB::Cyan,
  CRGB::White,
  CRGB::Black
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

// Music Notes (pitches.h equivalent)
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define REST     0

// Quantized durations (in ms) - from your corrected version
#define SIXTEENTH 125
#define EIGHTH 250
#define QUARTER 500
#define HALF 1000
#define WHOLE 2000

// Song State Machine
enum SongState {
  IDLE,
  PLAYING_SONG
};
SongState songState = IDLE;

// Song timing variables
unsigned long songStepStartTime = 0;
unsigned long songDelayEnd = 0;
int songStep = 0;
bool noteCurrentlyPlaying = false;
bool inDelay = false;

// Battery monitoring
unsigned long lastBatteryCheck = 0;
const unsigned long batteryCheckInterval = 10000; // Check every 10 seconds

// Force enable timer (for button interactions in bright light)
unsigned long forceEnableStartTime = 0;
const unsigned long forceEnableTimeout = 30000; // 30 seconds

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
void turnOffAllLEDs();
void updateDisplay();
void startSong();
void stopSong();
void updateSong();
void playKinderdorpSong();

void setup() {
  Serial.begin(115200);
  Serial.println("Kinderdorp PCB Starting...");
  
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
  
  Serial.println("Kinderdorp PCB Ready!");
  printPowerStatus();
}

void loop() {
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
  
  // Check force enable timeout
  if (ledsForceEnabled && (millis() - forceEnableStartTime > forceEnableTimeout)) {
    ledsForceEnabled = false;
    Serial.println("Force enable timeout - returning to automatic light control");
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
  } else if (voltage >= BATT_CR2032_MIN && voltage <= BATT_CR2032_MAX) {
    // CR2032 batteries detected
    currentPowerSource = POWER_CR2032;
    currentBrightness = BRIGHTNESS_BATTERY;
    wifiEnabled = false;
  } else if (voltage >= BATT_AAA_MIN && voltage <= BATT_AAA_MAX) {
    // AAA batteries detected
    currentPowerSource = POWER_AAA;
    currentBrightness = BRIGHTNESS_BATTERY;
    wifiEnabled = false;
  } else {
    // Unknown voltage - assume battery power for safety
    currentPowerSource = POWER_AAA;
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
      Serial.println("WiFi/Bluetooth disabled for battery operation");
    }
  }
}

void checkLightSensor() {
  int ldrReading = analogRead(LDR_PIN);
  
  // Apply hysteresis to prevent flickering
  if (!ledsEnabledByLight && ldrReading < (LDR_THRESHOLD - LDR_HYSTERESIS)) {
    // It's getting dark enough - enable LEDs
    ledsEnabledByLight = true;
    Serial.print("Light level decreased to ");
    Serial.print(ldrReading);
    Serial.println(" - LEDs enabled");
  } else if (ledsEnabledByLight && ldrReading > (LDR_THRESHOLD + LDR_HYSTERESIS)) {
    // It's getting too bright - disable LEDs (unless force enabled)
    ledsEnabledByLight = false;
    Serial.print("Light level increased to ");
    Serial.print(ldrReading);
    Serial.println(" - LEDs disabled by light sensor");
  }
  
  lastLDRReading = ldrReading;
}

bool shouldShowLEDs() {
  // Always show LEDs during song playback
  if (songState == PLAYING_SONG) {
    return true;
  }
  
  // Show LEDs if force enabled (button was pressed recently)
  if (ledsForceEnabled) {
    return true;
  }
  
  // Otherwise follow light sensor
  return ledsEnabledByLight;
}

void printPowerStatus() {
  Serial.print("Power Source: ");
  switch (currentPowerSource) {
    case POWER_USB:
      Serial.print("USB");
      break;
    case POWER_CR2032:
      Serial.print("CR2032");
      break;
    case POWER_AAA:
      Serial.print("AAA");
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
  // Enable LEDs temporarily when button is pressed (even in bright light)
  if (!shouldShowLEDs()) {
    ledsForceEnabled = true;
    forceEnableStartTime = millis();
    Serial.println("Button pressed - temporarily enabling LEDs");
  }
  
  if (currentMode == STATIC_COLOR) {
    currentColorIndex = (currentColorIndex + 1) % NUM_COLORS;
    if (currentColorIndex == NUM_COLORS - 1) {
      // After going through all colors, switch to first pattern mode
      currentMode = RAINBOW_MODE;
      Serial.println("Mode changed to RAINBOW");
    } else {
      Serial.print("Color changed to index: ");
      Serial.println(currentColorIndex);
    }
  } else {
    // Switch to next pattern mode
    currentMode = (DisplayMode)((int)currentMode + 1);
    if (currentMode > WAVE_MODE) {
      currentMode = STATIC_COLOR;
      currentColorIndex = 0;
      Serial.println("Mode reset to STATIC_COLOR");
    } else {
      Serial.print("Mode changed to: ");
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
      case OFF_MODE:
        turnOffAllLEDs();
        break;
    }
  }
}

void updateRainbowPattern() {
  breatheHue++;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(breatheHue + (i * 255 / NUM_LEDS), 255, 255);
  }
}

void updateSnakePattern() {
  turnOffAllLEDs();
  
  snakeHeadPos = (snakeHeadPos + 1) % NUM_LEDS;
  snakeHue += 2;
  
  for (int i = 0; i < snakeLength; i++) {
    int pos = (snakeHeadPos - i + NUM_LEDS) % NUM_LEDS;
    int brightness = 255 - (i * 255 / snakeLength);
    leds[pos] = CHSV(snakeHue, 255, brightness);
  }
}

void updateRandomBlinkPattern(unsigned long currentTime) {
  if (currentTime - lastRandomUpdate >= randomBlinkInterval) {
    lastRandomUpdate = currentTime;
    
    // Turn off old random LEDs
    for (int i = 0; i < 3; i++) {
      if (randomLEDs[i] < NUM_LEDS) {
        leds[randomLEDs[i]] = CRGB::Black;
      }
    }
    
    // Generate new random LEDs
    for (int i = 0; i < 3; i++) {
      randomLEDs[i] = random8(0, NUM_LEDS);
      randomHues[i] = random8(0, 255);
      leds[randomLEDs[i]] = CHSV(randomHues[i], 255, 255);
    }
  }
}

void updateChasePattern() {
  turnOffAllLEDs();
  
  leds[chasePos] = CHSV(chaseHue, 255, 255);
  
  chasePos = (chasePos + 1) % NUM_LEDS;
  chaseHue += 10;
}

void updateBreathePattern() {
  if (breatheIncreasing) {
    breatheBrightness += 3;
    if (breatheBrightness >= 250) {
      breatheIncreasing = false;
    }
  } else {
    breatheBrightness -= 3;
    if (breatheBrightness <= 5) {
      breatheIncreasing = true;
      breatheHue += 10;
    }
  }
  
  fill_solid(leds, NUM_LEDS, CHSV(breatheHue, 255, breatheBrightness));
}

void updateWavePattern() {
  waveOffset += 10;
  
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t sinBrightness = sin8(waveOffset + (i * 255 / NUM_LEDS));
    leds[i] = CHSV(waveHue, 255, sinBrightness);
  }
  
  waveHue++;
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

void startSong() {
  songStep = 0;
  songState = PLAYING_SONG;
  songStepStartTime = millis();
  noteCurrentlyPlaying = false;
  inDelay = false;
  turnOffAllLEDs();
  
  // Select color based on current mode
  if (currentMode != STATIC_COLOR) {
    currentColorIndex = random8(0, NUM_COLORS - 1); // Avoid black
  }
  
  Serial.println("Starting Kinderdorp song");
}

void stopSong() {
  songState = IDLE;
  noTone(BUZZER);
  updateDisplay();
  Serial.println("Song stopped");
}

void updateSong() {
  if (songState != PLAYING_SONG) return;
  
  unsigned long currentTime = millis();
  
  // Handle delay periods
  if (inDelay) {
    if (currentTime >= songDelayEnd) {
      inDelay = false;
      songStep++;
    }
    return;
  }
  
  // Execute the current song step
  playKinderdorpSong();
  
  // Progressive LED lighting during song
  float progress = (float)songStep / 155.0; // Total song steps
  int ledsToLight = (int)(progress * NUM_LEDS);
  if (ledsToLight > NUM_LEDS) ledsToLight = NUM_LEDS;
  
  for (int i = 0; i < ledsToLight; i++) {
    leds[i] = colorOptions[currentColorIndex];
  }
}

void playKinderdorpSong() {
  // Complete Kinderdorp song implementation based on your corrected version
  unsigned long currentTime = millis();
  
  switch (songStep) {
    // First verse - repeat
    // Edegc
    case 0: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 1: tone(BUZZER, NOTE_D4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 2: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 3: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 4: tone(BUZZER, NOTE_C4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 5: songDelayEnd = currentTime + EIGHTH; inDelay = true; break; // Pause
    
    // Edegc
    case 6: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 7: tone(BUZZER, NOTE_D4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 8: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 9: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 10: tone(BUZZER, NOTE_C4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 11: songDelayEnd = currentTime + EIGHTH; inDelay = true; break; // Pause
    
    // Eede
    case 12: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 13: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 14: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 15: tone(BUZZER, NOTE_D4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 16: tone(BUZZER, NOTE_E4, HALF); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 17: songDelayEnd = currentTime + HALF; inDelay = true; break; // Long pause
    
    // Second verse - repeat
    // Edegc
    case 18: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 19: tone(BUZZER, NOTE_D4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 20: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 21: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 22: tone(BUZZER, NOTE_C4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 23: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Edegc
    case 24: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 25: tone(BUZZER, NOTE_D4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 26: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 27: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 28: tone(BUZZER, NOTE_C4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 29: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Eede
    case 30: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 31: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 32: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 33: tone(BUZZER, NOTE_D4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 34: tone(BUZZER, NOTE_E4, HALF); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 35: songDelayEnd = currentTime + HALF; inDelay = true; break; // Long pause
    
    // Hutje hier,
    case 36: tone(BUZZER, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 37: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 38: tone(BUZZER, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 39: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Hutje daar,
    case 40: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 41: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 42: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 43: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Kinderdorp is bijna klaar
    case 44: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 45: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
    case 46: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 47: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 48: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 49: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
    case 50: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 51: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Groot en klein,
    case 52: tone(BUZZER, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 53: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 54: tone(BUZZER, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 55: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Zing maar mee,
    case 56: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 57: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 58: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 59: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Kinderdorp is echt... OK!
    case 60: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 61: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 62: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 63: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 64: tone(BUZZER, NOTE_G4, QUARTER); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 65: tone(BUZZER, NOTE_D4, QUARTER); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
    case 66: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 67: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Repeat chorus: Hutje hier,
    case 68: tone(BUZZER, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 69: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 70: tone(BUZZER, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 71: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Hutje daar,
    case 72: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 73: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 74: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 75: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Kinderdorp is bijna klaar
    case 76: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 77: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
    case 78: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 79: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 80: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 81: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
    case 82: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 83: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Groot en klein,
    case 84: tone(BUZZER, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 85: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 86: tone(BUZZER, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 87: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Zing maar mee,
    case 88: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 89: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 90: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 91: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Kinderdorp is echt... OK!
    case 92: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 93: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 94: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 95: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 96: tone(BUZZER, NOTE_G4, QUARTER); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 97: tone(BUZZER, NOTE_D4, QUARTER); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
    case 98: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 99: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Third repeat chorus: Hutje hier,
    case 100: tone(BUZZER, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 101: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 102: tone(BUZZER, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 103: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Hutje daar,
    case 104: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 105: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 106: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 107: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Kinderdorp is bijna klaar
    case 108: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 109: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
    case 110: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 111: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 112: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 113: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
    case 114: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 115: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Groot en klein,
    case 116: tone(BUZZER, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 117: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 118: tone(BUZZER, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 119: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Zing maar mee,
    case 120: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 121: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 122: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 123: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Final: Kinderdorp is echt... OK! (extended ending)
    case 124: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 125: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 126: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 127: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 128: tone(BUZZER, NOTE_G4, QUARTER); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 129: tone(BUZZER, NOTE_D4, QUARTER); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
    case 130: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 131: songDelayEnd = currentTime + WHOLE; inDelay = true; break; // Long final pause
    
    // Fourth and final repeat chorus: Hutje hier,
    case 132: tone(BUZZER, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 133: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 134: tone(BUZZER, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 135: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Hutje daar,
    case 136: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 137: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 138: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 139: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Kinderdorp is bijna klaar
    case 140: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 141: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
    case 142: tone(BUZZER, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 143: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 144: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 145: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
    case 146: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 147: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Groot en klein,
    case 148: tone(BUZZER, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 149: tone(BUZZER, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 150: tone(BUZZER, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 151: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Zing maar mee,
    case 152: tone(BUZZER, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 153: tone(BUZZER, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 154: tone(BUZZER, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 155: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
    
    // Grand finale: Kinderdorp is echt... OK!
    case 156: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 157: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 158: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
    case 159: tone(BUZZER, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
    case 160: tone(BUZZER, NOTE_G4, QUARTER); songDelayEnd = currentTime + HALF; inDelay = true; break;
    case 161: tone(BUZZER, NOTE_D4, QUARTER); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
    case 162: tone(BUZZER, NOTE_G4, WHOLE); songDelayEnd = currentTime + WHOLE; inDelay = true; break; // Extended final note
    
    default:
      // Song complete
      fill_solid(leds, NUM_LEDS, colorOptions[currentColorIndex]);
      delay(2000);
      songState = IDLE;
      updateDisplay();
      Serial.println("Kinderdorp song finished");
      break;
  }
}