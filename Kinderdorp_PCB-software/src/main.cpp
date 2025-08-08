
#include <Arduino.h>
#include "pitches.h"
#define BUZZER_PIN 10

// Quantized durations (in ms)
#define SIXTEENTH 125
#define EIGHTH    250
#define QUARTER   500
#define HALF      1000
#define WHOLE     2000


void Kinderdorplied();

void setup() {
  // Nothing to set up for buzzer
}

void loop() {
  Kinderdorplied();
  delay(1000000); // Pause before repeating
}

void Kinderdorplied() {
  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_D4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_G4, QUARTER); delay(HALF);
  // tone(BUZZER_PIN, NOTE_C4, QUARTER); delay(HALF);
  // delay(QUARTER);

  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_D4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_G4, QUARTER); delay(HALF);
  // tone(BUZZER_PIN, NOTE_C4, QUARTER); delay(HALF);
  // delay(QUARTER);

  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_D4, SIXTEENTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_E4, HALF); delay(HALF);
  // delay(QUARTER);


  // // and again
  // tone(BUZZER_PIN, NOTE_E4, QUARTER); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_D4, QUARTER); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_E4, QUARTER); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_G4, HALF); delay(HALF);
  // tone(BUZZER_PIN, NOTE_C4, QUARTER); delay(HALF);
  // delay(HALF);

  // tone(BUZZER_PIN, NOTE_E4, QUARTER); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_D4, QUARTER); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_E4, QUARTER); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_G4, HALF); delay(HALF);
  // tone(BUZZER_PIN, NOTE_C4, QUARTER); delay(HALF);
  // delay(HALF);

  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_D4, SIXTEENTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_E4, HALF); delay(HALF);


  // // hutje hier,
  // tone(BUZZER_PIN, NOTE_F4, SIXTEENTH); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_F4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_A4, HALF); delay(HALF);
  // delay (QUARTER);

  // // hutje daar,
  // tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_G4, HALF); delay(HALF);
  // delay (QUARTER);

  // // Kinderdorp is bijna klaar
  // tone(BUZZER_PIN, NOTE_G4, EIGHTH); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_G4, EIGHTH); delay(SIXTEENTH);
  // tone(BUZZER_PIN, NOTE_G4, EIGHTH); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_F4, EIGHTH); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_F4, EIGHTH); delay(SIXTEENTH);
  // tone(BUZZER_PIN, NOTE_G4, WHOLE); delay(EIGHTH);
  // delay(QUARTER);




  // // hutje hier,
  // tone(BUZZER_PIN, NOTE_F4, SIXTEENTH); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_F4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_A4, HALF); delay(HALF);
  // delay (QUARTER);

  // // hutje daar,
  // tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  // tone(BUZZER_PIN, NOTE_G4, HALF); delay(HALF);
  // delay (QUARTER);

  // // Kinderdorp is bijna klaar
  // tone(BUZZER_PIN, NOTE_G4, EIGHTH); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_G4, EIGHTH); delay(SIXTEENTH);
  // tone(BUZZER_PIN, NOTE_G4, EIGHTH); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_F4, EIGHTH); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(EIGHTH);
  // tone(BUZZER_PIN, NOTE_F4, EIGHTH); delay(SIXTEENTH);
  // tone(BUZZER_PIN, NOTE_G4, WHOLE); delay(EIGHTH);
  // delay(QUARTER);

  // Groot en klein,
  tone(BUZZER_PIN, NOTE_F4, SIXTEENTH); delay(EIGHTH);
  tone(BUZZER_PIN, NOTE_F4, EIGHTH); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_A4, HALF); delay(HALF);
  delay (QUARTER);

  // hutje daar,
  tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); delay(EIGHTH);
  tone(BUZZER_PIN, NOTE_E4, EIGHTH); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_G4, HALF); delay(HALF);
  delay (QUARTER);

  // Kinderdorp is echt... OK!
  tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); delay(EIGHTH);
  tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); delay(QUARTER);
  tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); delay(EIGHTH);
  tone(BUZZER_PIN, NOTE_G4, QUARTER); delay(HALF);
  tone(BUZZER_PIN, NOTE_D4, QUARTER); delay(SIXTEENTH);
  tone(BUZZER_PIN, NOTE_G4, HALF); delay(HALF);

}
