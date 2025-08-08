#include <Arduino.h>  // Required for PlatformIO
#include <FastLED.h>

// Credit:
// Midi to Arduino Converter - Andy Tran (extramaster), 2015
// https://www.extramaster.net/tools/midiToArduino/
//
// PROPER MUSICAL TRANSPOSITION - 2 octaves up (4x original frequencies)
// Maintains perfect musical intervals for proper harmony
// All notes minimum 250ms duration

int tonePin = 10;

void setup() {

}

void midi() {
    // Proper 2-octave transposition (4x original frequencies):
    // C(261Hz) → C6(1044Hz), D(293Hz) → D6(1172Hz), E(329Hz) → E6(1316Hz), 
    // F(349Hz) → F6(1396Hz), G(391Hz) → G6(1564Hz), A(440Hz) → A6(1760Hz)

    tone(tonePin, 1316, 250);  // E4→E6 - extended from 44.92ms
    delay(250);
    delay(320.3125);
    tone(tonePin, 1172, 250);  // D4→D6 - extended from 34.20ms
    delay(250);
    delay(336.458333333);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 20.42ms
    delay(250);
    delay(696.354166667);
    tone(tonePin, 1564, 250);  // G4→G6 - extended from 10.21ms
    delay(250);
    delay(1118.22916667);
    tone(tonePin, 1044, 250);  // C4→C6 - extended from 7.66ms
    delay(250);
    delay(355.208333333);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 22.46ms
    delay(250);
    delay(368.229166667);
    tone(tonePin, 1172, 250);  // D4→D6 - extended from 13.78ms
    delay(250);
    delay(365.104166667);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 10.21ms
    delay(250);
    tone(tonePin, 1564, 705.395833333);  // G4→G6 - kept original (already long)
    delay(719.791666667);
    delay(4.16666666667);
    tone(tonePin, 1044, 1068.8125);  // C4→C6 - kept original (already long)
    delay(1090.625);
    delay(436.979166667);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 158.74ms
    delay(250);
    delay(427.083333333);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 116.38ms
    delay(250);
    delay(492.1875);
    delay(631.770833333);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 4.08ms
    delay(250);
    delay(383.854166667);
    tone(tonePin, 1172, 250);  // D4→D6 - extended from 6.13ms
    delay(250);
    tone(tonePin, 1316, 1441.92708333);  // E4→E6 - kept original (already long)
    delay(1471.35416667);
    delay(532.8125);
    delay(345.833333333);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 15.31ms
    delay(250);
    delay(334.895833333);
    tone(tonePin, 1172, 250);  // D4→D6 - extended from 21.44ms
    delay(250);
    delay(318.229166667);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 22.46ms
    delay(250);
    delay(658.333333333);
    tone(tonePin, 1564, 250);  // G4→G6 - extended from 6.13ms
    delay(250);
    tone(tonePin, 1044, 959.583333333);  // C4→C6 - kept original (already long)
    delay(979.166666667);
    delay(15.625);
    delay(352.083333333);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 7.66ms
    delay(250);
    delay(345.833333333);
    delay(342.708333333);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 16.33ms
    delay(250);
    delay(654.166666667);
    tone(tonePin, 1564, 250);  // G4→G6 - extended from 5.10ms
    delay(250);
    tone(tonePin, 1044, 1248.47916667);  // C4→C6 - kept original (already long)
    delay(1273.95833333);
    delay(138.020833333);
    tone(tonePin, 1316, 335.854166667);  // E4→E6 - kept original (already long)
    delay(342.708333333);
    delay(315.625);
    tone(tonePin, 1316, 272.052083333);  // E4→E6 - kept original (already long)
    delay(277.604166667);
    delay(355.729166667);
    delay(623.4375);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 7.66ms
    delay(250);
    delay(363.541666667);
    tone(tonePin, 1172, 250);  // D4→D6 - extended from 1.02ms
    delay(250);
    tone(tonePin, 1316, 1749.19791667);  // E4→E6 - kept original (already long)
    delay(1784.89583333);
    delay(1050.0);
    tone(tonePin, 1396, 250);  // F4→F6 - extended from 149.55ms
    delay(250);
    delay(145.833333333);
    tone(tonePin, 1396, 556.354166667);  // F4→F6 - kept original (already long)
    delay(567.708333333);
    delay(4.16666666667);
    tone(tonePin, 1760, 1349.54166667);  // A4→A6 - kept original (already long)
    delay(1377.08333333);
    delay(46.875);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 126.07ms
    delay(250);
    delay(205.208333333);
    delay(573.958333333);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 10.72ms
    delay(250);
    tone(tonePin, 1564, 1316.875);  // G4→G6 - kept original (already long)
    delay(1343.75);
    delay(196.354166667);
    tone(tonePin, 1564, 352.697916667);  // G4→G6 - kept original (already long)
    delay(359.895833333);
    delay(231.25);
    tone(tonePin, 1564, 250);  // G4→G6 - extended from 117.40ms
    delay(250);
    delay(195.3125);
    delay(629.6875);
    tone(tonePin, 1564, 250);  // G4→G6 - extended from 17.86ms
    delay(250);
    delay(307.291666667);
    tone(tonePin, 1396, 250);  // F4→F6 - extended from 25.52ms
    delay(250);
    delay(298.958333333);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 22.46ms
    delay(250);
    tone(tonePin, 1396, 311.354166667);  // F4→F6 - kept original (already long)
    delay(317.708333333);
    delay(4.6875);
    tone(tonePin, 1564, 1696.625);  // G4→G6 - kept original (already long)
    delay(1731.25);
    delay(552.604166667);
    tone(tonePin, 1396, 250);  // F4→F6 - extended from 135.77ms
    delay(250);
    delay(171.875);
    tone(tonePin, 1396, 586.46875);  // F4→F6 - kept original (already long)
    delay(598.4375);
    delay(2.08333333333);
    tone(tonePin, 1760, 1400.07291667);  // A4→A6 - kept original (already long)
    delay(1428.64583333);
    delay(11.9791666667);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 115.35ms
    delay(250);
    delay(177.604166667);
    tone(tonePin, 1316, 598.71875);  // E4→E6 - kept original (already long)
    delay(610.9375);
    delay(17.7083333333);
    tone(tonePin, 1564, 1225.51041667);  // G4→G6 - kept original (already long)
    delay(1250.52083333);
    delay(249.479166667);
    tone(tonePin, 1564, 382.302083333);  // G4→G6 - kept original (already long)
    delay(390.104166667);
    delay(236.979166667);
    tone(tonePin, 1564, 250);  // G4→G6 - extended from 157.72ms
    delay(250);
    delay(168.229166667);
    tone(tonePin, 1564, 433.854166667);  // G4→G6 - kept original (already long)
    delay(442.708333333);
    delay(160.416666667);
    tone(tonePin, 1564, 250);  // G4→G6 - extended from 139.34ms
    delay(250);
    delay(202.083333333);
    tone(tonePin, 1564, 541.552083333);  // G4→G6 - kept original (already long)
    delay(552.604166667);
    delay(742.708333333);
    delay(708.854166667);
    tone(tonePin, 1172, 250);  // D4→D6 - extended from 2.55ms
    delay(250);
    tone(tonePin, 1564, 941.71875);  // G4→G6 - kept original (already long)
    delay(960.9375);
    delay(284.895833333);
    tone(tonePin, 1396, 250);  // F4→F6 - extended from 150.57ms
    delay(250);
    delay(177.604166667);
    delay(617.708333333);
    tone(tonePin, 1396, 250);  // F4→F6 - extended from 8.68ms
    delay(250);
    tone(tonePin, 1760, 1390.88541667);  // A4→A6 - kept original (already long)
    delay(1419.27083333);
    delay(53.125);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 122.5ms
    delay(250);
    delay(200.520833333);
    tone(tonePin, 1316, 582.385416667);  // E4→E6 - kept original (already long)
    delay(594.270833333);
    delay(13.5416666667);
    tone(tonePin, 1564, 1281.65625);  // G4→G6 - kept original (already long)
    delay(1307.8125);
    delay(183.854166667);
    tone(tonePin, 1564, 405.78125);  // G4→G6 - kept original (already long)
    delay(414.0625);
    delay(175.0);
    tone(tonePin, 1564, 250);  // G4→G6 - extended from 124.03ms
    delay(250);
    delay(176.5625);
    delay(622.395833333);
    tone(tonePin, 1564, 250);  // G4→G6 - extended from 40.32ms
    delay(250);
    delay(271.354166667);
    tone(tonePin, 1396, 250);  // F4→F6 - extended from 34.20ms
    delay(250);
    delay(278.125);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 26.54ms
    delay(250);
    delay(282.8125);
    tone(tonePin, 1396, 250);  // F4→F6 - extended from 6.64ms
    delay(250);
    tone(tonePin, 1564, 1866.08333333);  // G4→G6 - kept original (already long)
    delay(1904.16666667);
    delay(494.270833333);
    tone(tonePin, 1396, 250);  // F4→F6 - extended from 166.91ms
    delay(250);
    delay(135.9375);
    delay(555.729166667);
    tone(tonePin, 1396, 250);  // F4→F6 - extended from 15.82ms
    delay(250);
    tone(tonePin, 1760, 1403.13541667);  // A4→A6 - kept original (already long)
    delay(1431.77083333);
    delay(30.7291666667);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 107.70ms
    delay(250);
    delay(210.416666667);
    delay(615.625);
    tone(tonePin, 1316, 250);  // E4→E6 - extended from 2.55ms
    delay(250);
    tone(tonePin, 1564, 1186.71875);  // G4→G6 - kept original (already long)
    delay(1210.9375);
    delay(272.395833333);
    tone(tonePin, 1564, 383.322916667);  // G4→G6 - kept original (already long)
    delay(391.145833333);
    delay(195.833333333);
    tone(tonePin, 1564, 250);  // G4→G6 - extended from 169.46ms
    delay(250);
    delay(167.708333333);
    tone(tonePin, 1564, 461.416666667);  // G4→G6 - kept original (already long)
    delay(470.833333333);
    delay(116.666666667);
    tone(tonePin, 1564, 250);  // G4→G6 - extended from 172.01ms
    delay(250);
    delay(160.9375);
    tone(tonePin, 1564, 305.229166667);  // G4→G6 - kept original (already long)
    delay(311.458333333);
    delay(971.875);
    tone(tonePin, 1172, 663.541666667);  // D4→D6 - kept original (already long)
    delay(677.083333333);
    delay(5.20833333333);
    tone(tonePin, 1564, 1816.57291667);  // G4→G6 - kept original (already long)
    delay(1853.64583333);

}

void loop() {
    // Play midi
    midi();
}