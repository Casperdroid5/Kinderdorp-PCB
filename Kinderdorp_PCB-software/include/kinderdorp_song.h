#ifndef KINDERDORP_SONG_H
#define KINDERDORP_SONG_H

#include <Arduino.h>

// Hardware Configuration
#define BUZZER_PIN 10  // Buzzer pin definition

// Musical Notes
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

// Note Durations (in ms)
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

extern SongState songState;
extern unsigned long songStepStartTime;
extern unsigned long songDelayEnd;
extern int songStep;
extern bool noteCurrentlyPlaying;
extern bool inDelay;

// Function declarations
void initSong();
void stopSong();
void updateSongCore();
void playKinderdorpSong();

#endif // KINDERDORP_SONG_H
