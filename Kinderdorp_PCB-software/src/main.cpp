
#include <Arduino.h>
#include "pitches.h"
#define BUZZER_PIN 10

// Quantized durations (in ms)
#define SIXTEENTH 125
#define EIGHTH    250
#define QUARTER   500
#define HALF      1000
#define WHOLE     2000


void happyBirthday();

void setup() {
  // Nothing to set up for buzzer
}

void loop() {
  happyBirthday();
  delay(2000); // Pause before repeating
}

void happyBirthday() {
  // "Happy Birthday to you"
  tone(BUZZER_PIN, NOTE_C4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_C4, EIGHTH);  delay(EIGHTH);
  tone(BUZZER_PIN, NOTE_D4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_C4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_F4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_E4, HALF);    delay(HALF);

  // "Happy Birthday to you"
  tone(BUZZER_PIN, NOTE_C4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_C4, EIGHTH);  delay(EIGHTH);
  tone(BUZZER_PIN, NOTE_D4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_C4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_G4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_F4, HALF);    delay(HALF);

  // "Happy Birthday dear [Name]"
  tone(BUZZER_PIN, NOTE_C4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_C4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_C5, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_A4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_F4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_E4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_D4, HALF);    delay(HALF);

  // "Happy Birthday to you"
  tone(BUZZER_PIN, NOTE_AS4, QUARTER); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_AS4, EIGHTH);  delay(EIGHTH);
  tone(BUZZER_PIN, NOTE_A4, QUARTER);  delay(QUARTER);
  tone(BUZZER_PIN, NOTE_F4, QUARTER);  delay(QUARTER);
  tone(BUZZER_PIN, NOTE_G4, QUARTER);  delay(QUARTER);
  tone(BUZZER_PIN, NOTE_F4, HALF);     delay(HALF);
}
