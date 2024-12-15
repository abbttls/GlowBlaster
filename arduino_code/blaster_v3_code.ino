/*
GlowBlaster Version 3 Arduino Code

This Arduino code is embedded logic for the GlowBlaster project, created and maintained by Louis Abbott.

GlowBlaster pulses a 405 nm laser, producing green dots on glow-in-the-dark targets.
This code implements 5 firing modes and reloading functionality for the blaster.

It is assumed that this will be uploaded on an Arduino Nano board wired according to this project's
build guide with pin usage matching the variable assignments below. This version of the blaster code
utilizes two buttons in addition to a trigger. To compile/upload successfully, the Arduino PCM library
must be installed (https://docs.arduino.cc/libraries/pcm/).
*/

#include <PCM.h>

// custom generated laser noise data
const unsigned char sample[] PROGMEM = {
228, 228, 0, 228, 228, 0, 228, 228, 0, 228, 228, 0, 228, 228, 0, 228, 228, 0, 228, 228, 0, 219, 219, 0, 0, 219, 219, 0, 0, 219, 219, 0, 0, 219, 219, 0, 0, 219, 219, 0, 0, 219, 219, 0, 0, 219, 219, 0, 0, 210, 210, 0, 210, 210, 0, 210, 210, 0, 210, 210, 0, 210, 210, 0, 210, 210, 0, 201, 201, 201, 0, 0, 0, 201, 201, 201, 0, 0, 0, 201, 201, 201, 0, 0, 0, 201, 201, 201, 0, 0, 0, 201, 201, 201, 0, 0, 0, 201, 201, 201, 0, 0, 0, 192, 192, 192, 192, 0, 0, 0, 192, 192, 192, 192, 0, 0, 0, 192, 192, 192, 192, 0, 0, 0, 192, 192, 192, 192, 0, 0, 0, 192, 192, 192, 192, 0, 0, 0, 192, 192, 192, 192, 0, 0, 0, 183, 183, 183, 183, 0, 0, 0, 0, 183, 183, 183, 183, 0, 0, 0, 0, 183, 183, 183, 183, 0, 0, 0, 0, 183, 183, 183, 183, 0, 0, 0, 0, 183, 183, 183, 183, 0, 0, 0, 0, 183, 183, 183, 183, 0, 0, 0, 0, 174, 174, 174, 174, 0, 0, 0, 174, 174, 174, 174, 0, 0, 0, 174, 174, 174, 174, 0, 0, 0, 174, 174, 174, 174, 0, 0, 0, 174, 174, 174, 174, 0, 0, 0, 174, 174, 174, 174, 0, 0, 0, 165, 165, 165, 165, 165, 0, 0, 0, 0, 0, 165, 165, 165, 165, 165, 0, 0, 0, 0, 0, 165, 165, 165, 165, 165, 0, 0, 0, 0, 0, 165, 165, 165, 165, 165, 0, 0, 0, 0, 0, 165, 165, 165, 165, 165, 0, 0, 0, 0, 0, 165, 165, 165, 165, 165, 0, 0, 0, 0, 0, 156, 156, 156, 156, 156, 156, 0, 0, 0, 0, 0, 156, 156, 156, 156, 156, 156, 0, 0, 0, 0, 0, 156, 156, 156, 156, 156, 156, 0, 0, 0, 0, 0, 156, 156, 156, 156, 156, 156, 0, 0, 0, 0, 0, 156, 156, 156, 156, 156, 156, 0, 0, 0, 0, 0, 156, 156, 156, 156, 156, 156, 0, 0, 0, 0, 0, 147, 147, 147, 147, 147, 147, 0, 0, 0, 0, 0, 0, 147, 147, 147, 147, 147, 147, 0, 0, 0, 0, 0, 0, 147, 147, 147, 147, 147, 147, 0, 0, 0, 0, 0, 0, 147, 147, 147, 147, 147, 147, 0, 0, 0, 0, 0, 0, 147, 147, 147, 147, 147, 147, 0, 0, 0, 0, 0, 0, 138, 138, 138, 138, 138, 138, 0, 0, 0, 0, 0, 138, 138, 138, 138, 138, 138, 0, 0, 0, 0, 0, 138, 138, 138, 138, 138, 138, 0, 0, 0, 0, 0, 138, 138, 138, 138, 138, 138, 0, 0, 0, 0, 0, 138, 138, 138, 138, 138, 138, 0, 0, 0, 0, 0, 129, 129, 129, 129, 129, 129, 129, 0, 0, 0, 0, 0, 0, 0, 129, 129, 129, 129, 129, 129, 129, 0, 0, 0, 0, 0, 0, 0, 129, 129, 129, 129, 129, 129, 129, 0, 0, 0, 0, 0, 0, 0, 129, 129, 129, 129, 129, 129, 129, 0, 0, 0, 0, 0, 0, 0, 129, 129, 129, 129, 129, 129, 129, 0, 0, 0, 0, 0, 0, 0, 129, 129, 129, 129, 129, 129, 129, 0, 0, 0, 0, 0, 0, 0, 120, 120, 120, 120, 120, 120, 120, 120, 0, 0, 0, 0, 0, 0, 0, 120, 120, 120, 120, 120, 120, 120, 120, 0, 0, 0, 0, 0, 0, 0, 120, 120, 120, 120, 120, 120, 120, 120, 0, 0, 0, 0, 0, 0, 0, 120, 120, 120, 120, 120, 120, 120, 120, 0, 0, 0, 0, 0, 0, 0, 120, 120, 120, 120, 120, 120, 120, 120, 0, 0, 0, 0, 0, 0, 0, 120, 120, 120, 120, 120, 120, 120, 120, 0, 0, 0, 0, 0, 0, 0, 111, 111, 111, 111, 111, 111, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 111, 111, 111, 111, 111, 111, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 111, 111, 111, 111, 111, 111, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 111, 111, 111, 111, 111, 111, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 111, 111, 111, 111, 111, 111, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 111, 111, 111, 111, 111, 111, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 102, 102, 102, 102, 102, 102, 102, 102, 0, 0, 0, 0, 0, 0, 0, 102, 102, 102, 102, 102, 102, 102, 102, 0, 0, 0, 0, 0, 0, 0, 102, 102, 102, 102, 102, 102, 102, 102, 0, 0, 0, 0, 0, 0, 0, 102, 102, 102, 102, 102, 102, 102, 102, 0, 0, 0, 0, 0, 0, 0, 102, 102, 102, 102, 102, 102, 102, 102, 0, 0, 0, 0, 0, 0, 0, 102, 102, 102, 102, 102, 102, 102, 102, 0, 0, 0, 0, 0, 0, 0, 93, 93, 93, 93, 93, 93, 93, 93, 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 93, 93, 93, 93, 93, 93, 93, 93, 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 93, 93, 93, 93, 93, 93, 93, 93, 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 93, 93, 93, 93, 93, 93, 93, 93, 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 93, 93, 93, 93, 93, 93, 93, 93, 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 93, 93, 93, 93, 93, 93, 93, 93, 93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 0, 0, 0, 0, 0, 0, 0, 0, 0, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const int sampleSize = sizeof(sample);

const byte laserPin = 9;
const byte vibratorPin = 7;
const byte speakerPin = 11;

const byte triggerPin = 5;
const byte leftButtonPin = 4;     // button used for switching modes
const byte rightButtonPin = 3;    // button used for reloading

const byte ledPin1 = A1;          // leftmost LED
const byte ledPin2 = A2;
const byte ledPin3 = A3;

byte roundNumber;                 // stores the round the gun is on
byte modeNumber;                  // stores the mode as a number between 1 and 5 (inclusive)
bool switchingModes = false;      // tells the program whether or not to switch modes
bool reloadingOn = true;          // reloading ON by default

void setup() {
  pinMode(laserPin, OUTPUT);
  pinMode(vibratorPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  
  pinMode(triggerPin, INPUT_PULLUP);
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);

  digitalWrite(ledPin1, HIGH);
  tone(speakerPin, 400, 50);
  delay(50);
  tone(speakerPin, 550, 50);
  delay(50);
  
  digitalWrite(ledPin2, HIGH);
  tone(speakerPin, 750, 50);
  delay(50);
  tone(speakerPin, 1150, 50);
  delay(50);

  digitalWrite(ledPin3, HIGH);
  tone(speakerPin, 1950, 50);
  delay(60);
}

void loop() {
  pistol();
  AR();
  burst();
  SMG();
  pistolNightMode();
}

// fire the laser one time with sound
void shootLaser() {
  // briely turns on laser diode and plays sound
  digitalWrite(laserPin, HIGH);
  startPlayback(sample, sampleSize);
  digitalWrite(vibratorPin, HIGH);
  digitalWrite(A5, HIGH);
  delay(8);
  digitalWrite(laserPin, LOW);
  delay(52);
  digitalWrite(vibratorPin, LOW);
  digitalWrite(A5, LOW);
}

// play a noise, reload gun, and wait for release of button
void reload() {
  // sounds start at 400 Hz and move up by 50, then 100, then 200, and so on.
  tone(speakerPin, 400, 50);
  delay(50);
  tone(speakerPin, 550, 50);
  delay(50);
  tone(speakerPin, 750, 50);
  delay(50);
  tone(speakerPin, 1150, 50);
  delay(50);
  tone(speakerPin, 1950, 50);
  delay(50);
  roundNumber = 1;
}

// check left button in order to toggle reloading or change modes
void checkLeftButton() {
  if (digitalRead(leftButtonPin) == LOW) {
    stopPlayback();
    long int pressTime = millis();
    
    // To toggle reloadig, the user must hold the button for a minimum of 1 second (1000 milliseconds)
    bool toggled = false;
    while (digitalRead(leftButtonPin) == LOW) {
      if ((millis() - pressTime) >= 1000) {
        toggled = true;
        if (reloadingOn) {
          reloadingOn = false;
          tone(speakerPin, 600, 300);
          delay(300);
          tone(speakerPin, 400, 300);
          delay(300);
          while (true) {
            if (digitalRead(leftButtonPin) == HIGH) {
              delay(100);
              break;
            }
          }
          break;
        }
        else {
          reloadingOn = true;
          tone(speakerPin, 400, 300);
          delay(300);
          tone(speakerPin, 600, 300);
          delay(300);
          while (true) {
            if (digitalRead(leftButtonPin) == HIGH) {
              delay(100);
              break;
            }
          }
          break;
        }
      }
    }
    if (toggled == false) {
      tone(speakerPin, 400, 50);
      delay(100);
      switchingModes = true;
    }
  }
}

// check right button for reloading
void checkRightButton() {
  if (digitalRead(rightButtonPin) == LOW) {
    long int pressTime = millis();

    // To reload, the user must hold the button for a minimum of 1 second (1000 milliseconds)
    while (digitalRead(rightButtonPin) == LOW) {
      if (((millis() - pressTime) >= 1000) && modeNumber != 5) {
        reload();
        while (true) {
          if (digitalRead(rightButtonPin) == HIGH) {
            delay(50);  // debounces button input and allows for momentary release of button
            break;
          }
        }
        break;
      }
    }
  }
}

// check right and left buttons for reloading and settings adjustment
void checkButtons(int maxRound) {
  checkLeftButton();
  if (reloadingOn) {
    checkRightButton();
    triggerReloadCheck(maxRound);
  }
}

// override reloading requirement if reloadingOn is false
void reloadOverride() {
  if (!reloadingOn) {
    roundNumber = 1;
  }
}

// special trigger reloading
void triggerReloadCheck(int maxRound) {
  if ((digitalRead(triggerPin) == LOW) && (roundNumber > maxRound) && reloadingOn) {
    long int pressTime = millis();

    // To reload, the user must hold the trigger for a minimum of 1 second (1000 milliseconds)
    while (digitalRead(triggerPin) == LOW) {
      if (((millis() - pressTime) >= 1000)) {
        reload();
        while (true) {
          if (digitalRead(triggerPin) == HIGH) {
            delay(100); // debouncing
            break;
          }
        }
        break;
      }
    }
  }
}

// semi-automatic pistol mode
void pistol() {
  modeNumber = 1;
  roundNumber = 1;
  while (!switchingModes) {
    if ((digitalRead(triggerPin) == LOW) && (roundNumber <= 16)) {
      shootLaser();
      roundNumber++;
      while (true) {
        if (digitalRead(triggerPin) == HIGH) {
          delay(40); // debounces button input and allows for momentary release of button
          if (digitalRead(triggerPin) == HIGH) {
            break;
          }
        }
      }  
    }
    reloadOverride();
    checkButtons(16);
  }
  switchingModes = false; // resets switchingModes for the next mode
}

// assault rifle mode
void AR() {
  modeNumber = 2;
  roundNumber = 1;
  while (!switchingModes) {
    if ((digitalRead(triggerPin) == LOW) && (roundNumber <= 30)) {
      shootLaser();
      roundNumber++;
      delay(120); // time based on legendary assault rifle in Fortnite
    }
    reloadOverride();
    checkButtons(30);
  }
  switchingModes = false;  // resets switchingModes for the next mode
}

// burst assault rifle mode
void burst() {
  modeNumber = 3;
  roundNumber = 1;
  while (!switchingModes) {
    if ((digitalRead(triggerPin) == LOW) && (roundNumber <= 30)) {
      // fires a burst of 3 shots, even if user releases trigger before end of burst
      shootLaser();
      delay(40); // time based on legendary burst assault rifle in Fortnite
      shootLaser();
      delay(40);
      shootLaser();
      roundNumber += 3;
      delay(340); // time based on legendary burst assault rifle in Fortnite
    }
    reloadOverride();
    checkButtons(30);
  }
  switchingModes = false; // reset switchingModes for next mode
}

// rapid fire/submachine gun mode
void SMG() {
  modeNumber = 4;
  roundNumber = 1;
  while (!switchingModes) {
    if ((digitalRead(triggerPin) == LOW) && (roundNumber <= 30)) {
      shootLaser();
      roundNumber++;
      delay(20); // time based on legendary submachine gun in Fortnite
    }
    reloadOverride();
    checkButtons(30);
  }
  switchingModes = false; // resets switchingModes for the next mode
}

// silent pistol mode with no reloading
void pistolNightMode() {
  modeNumber = 5;
  roundNumber = 2;
  while (!switchingModes) {
    if (digitalRead(triggerPin) == LOW) {
      digitalWrite(laserPin, HIGH);
      digitalWrite(vibratorPin, HIGH);
      delay(8);  // 8 milliseconds minimized duration while maintaining dot brightness.
      digitalWrite(laserPin, LOW);
      delay(52);  // because 70 is a nice number.
      digitalWrite(vibratorPin, LOW);
      while (true) {
        if (digitalRead(triggerPin) == HIGH) {
          delay(50); // debounces button input and allows for momentary release of button
          if (digitalRead(triggerPin) == HIGH) {
            break;
          }
        }
      }
    }
    checkButtons(30);
  }
  switchingModes = false; // resets switchingModes for the next mode
}
