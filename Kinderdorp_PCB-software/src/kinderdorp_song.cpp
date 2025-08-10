#include "kinderdorp_song.h"

// Song State Variables
SongState songState = IDLE;
unsigned long songStepStartTime = 0;
unsigned long songDelayEnd = 0;
int songStep = 0;
bool noteCurrentlyPlaying = false;
bool inDelay = false;

void initSong() {
    songStep = 0;
    songState = PLAYING_SONG;
    songStepStartTime = millis();
    noteCurrentlyPlaying = false;
    inDelay = false;
    
    Serial.println("Starting Kinderdorp song");
}

void stopSong() {
    songState = IDLE;
    noTone(BUZZER_PIN);
    Serial.println("Song stopped");
}

void updateSongCore() {
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
}

void playKinderdorpSong() {
    unsigned long currentTime = millis();
    
    switch (songStep) {
        // First verse - repeat
        // Edegc
        case 0: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 1: tone(BUZZER_PIN, NOTE_D4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 2: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 3: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 4: tone(BUZZER_PIN, NOTE_C4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 5: songDelayEnd = currentTime + EIGHTH; inDelay = true; break; // Pause
        
        // Edegc
        case 6: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 7: tone(BUZZER_PIN, NOTE_D4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 8: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 9: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 10: tone(BUZZER_PIN, NOTE_C4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 11: songDelayEnd = currentTime + EIGHTH; inDelay = true; break; // Pause
        
        // Eede
        case 12: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 13: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 14: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 15: tone(BUZZER_PIN, NOTE_D4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 16: tone(BUZZER_PIN, NOTE_E4, HALF); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 17: songDelayEnd = currentTime + HALF; inDelay = true; break; // Long pause
        
        // Second verse - repeat
        // Edegc
        case 18: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 19: tone(BUZZER_PIN, NOTE_D4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 20: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 21: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 22: tone(BUZZER_PIN, NOTE_C4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 23: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Continue with more song cases
        case 24: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 25: tone(BUZZER_PIN, NOTE_D4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 26: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 27: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 28: tone(BUZZER_PIN, NOTE_C4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 29: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Eede
        case 30: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 31: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 32: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 33: tone(BUZZER_PIN, NOTE_D4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 34: tone(BUZZER_PIN, NOTE_E4, HALF); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 35: songDelayEnd = currentTime + HALF; inDelay = true; break; // Long pause
        
        // Hutje hier,
        case 36: tone(BUZZER_PIN, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 37: tone(BUZZER_PIN, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 38: tone(BUZZER_PIN, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 39: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Hutje daar,
        case 40: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 41: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 42: tone(BUZZER_PIN, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 43: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Kinderdorp is bijna klaar - Continue with more cases...
        case 44: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 45: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
        case 46: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 47: tone(BUZZER_PIN, NOTE_F4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 48: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 49: tone(BUZZER_PIN, NOTE_F4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
        case 50: tone(BUZZER_PIN, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 51: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Groot en klein,
        case 52: tone(BUZZER_PIN, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 53: tone(BUZZER_PIN, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 54: tone(BUZZER_PIN, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 55: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Zing maar mee,
        case 56: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 57: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 58: tone(BUZZER_PIN, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 59: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Kinderdorp is echt... OK!
        case 60: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 61: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 62: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 63: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 64: tone(BUZZER_PIN, NOTE_G4, QUARTER); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 65: tone(BUZZER_PIN, NOTE_D4, QUARTER); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
        case 66: tone(BUZZER_PIN, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 67: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Repeat chorus: Hutje hier,
        case 68: tone(BUZZER_PIN, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 69: tone(BUZZER_PIN, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 70: tone(BUZZER_PIN, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 71: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Hutje daar,
        case 72: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 73: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 74: tone(BUZZER_PIN, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 75: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Kinderdorp is bijna klaar
        case 76: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 77: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
        case 78: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 79: tone(BUZZER_PIN, NOTE_F4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 80: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 81: tone(BUZZER_PIN, NOTE_F4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
        case 82: tone(BUZZER_PIN, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 83: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Groot en klein,
        case 84: tone(BUZZER_PIN, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 85: tone(BUZZER_PIN, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 86: tone(BUZZER_PIN, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 87: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Zing maar mee,
        case 88: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 89: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 90: tone(BUZZER_PIN, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 91: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Kinderdorp is echt... OK!
        case 92: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 93: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 94: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 95: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 96: tone(BUZZER_PIN, NOTE_G4, QUARTER); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 97: tone(BUZZER_PIN, NOTE_D4, QUARTER); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
        case 98: tone(BUZZER_PIN, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 99: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Third repeat chorus: Hutje hier,
        case 100: tone(BUZZER_PIN, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 101: tone(BUZZER_PIN, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 102: tone(BUZZER_PIN, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 103: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Hutje daar,
        case 104: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 105: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 106: tone(BUZZER_PIN, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 107: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Kinderdorp is bijna klaar
        case 108: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 109: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
        case 110: tone(BUZZER_PIN, NOTE_G4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 111: tone(BUZZER_PIN, NOTE_F4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 112: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 113: tone(BUZZER_PIN, NOTE_F4, EIGHTH); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
        case 114: tone(BUZZER_PIN, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 115: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Groot en klein,
        case 116: tone(BUZZER_PIN, NOTE_F4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 117: tone(BUZZER_PIN, NOTE_F4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 118: tone(BUZZER_PIN, NOTE_A4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 119: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Zing maar mee,
        case 120: tone(BUZZER_PIN, NOTE_E4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 121: tone(BUZZER_PIN, NOTE_E4, EIGHTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 122: tone(BUZZER_PIN, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 123: songDelayEnd = currentTime + QUARTER; inDelay = true; break; // Pause
        
        // Final: Kinderdorp is echt... OK! (extended ending)
        case 124: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 125: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 126: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 127: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 128: tone(BUZZER_PIN, NOTE_G4, QUARTER); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 129: tone(BUZZER_PIN, NOTE_D4, QUARTER); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
        case 130: tone(BUZZER_PIN, NOTE_G4, HALF); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 131: songDelayEnd = currentTime + WHOLE; inDelay = true; break; // Long final pause
        
        // Grand finale: Kinderdorp is echt... OK!
        case 132: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 133: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 134: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + QUARTER; inDelay = true; break;
        case 135: tone(BUZZER_PIN, NOTE_G4, SIXTEENTH); songDelayEnd = currentTime + EIGHTH; inDelay = true; break;
        case 136: tone(BUZZER_PIN, NOTE_G4, QUARTER); songDelayEnd = currentTime + HALF; inDelay = true; break;
        case 137: tone(BUZZER_PIN, NOTE_D4, QUARTER); songDelayEnd = currentTime + SIXTEENTH; inDelay = true; break;
        case 138: tone(BUZZER_PIN, NOTE_G4, WHOLE); songDelayEnd = currentTime + WHOLE; inDelay = true; break; // Extended final note
        case 139: songDelayEnd = currentTime + WHOLE; inDelay = true; break; // Final pause
        default:
            // Song complete
            songState = IDLE;
            Serial.println("Kinderdorp song finished");
            break;
    }
}
