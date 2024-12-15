/*
GlowBlaster Version 1 Arduino Code

This Arduino code is embedded logic for the GlowBlaster project, created and maintained by Louis Abbott.

GlowBlaster pulses a 405 nm laser, producing green dots on glow-in-the-dark targets.
This code implements 5 firing modes and reloading functionality for the blaster.

WARNING: DO NOT USE IF BUILDING FROM build_instructions.pdf. This code is intended for use with
an earlier version of the blaster with a buzzer instead of a speaker and a SINGLE button for
both reloading and mode switching. It exists for use only with my own prototype and as a
reference for anyone looking to simplify the aforementioned features.
*/

const byte laserPin = 9;
const byte buzzerPin = A2;
const byte triggerPin = 7;
const byte buttonPin = 5;     // button used for reloading and switching modes
byte roundNumber;             // stores the round the gun is on
byte modeNumber;              // stores the mode as a number between 1 and 5 (inclusive)
bool switchingModes = false;  // tells the program whether or not to switch modes

void setup() {
  pinMode(laserPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  pistol();
  AR();
  burst();
  SMG();
  pistolNightMode();
}

// fire one shot of the laser, play fire noise
void shootLaser() {
  digitalWrite(laserPin, HIGH);
  tone(buzzerPin, 2000, 8);
  delay(8);  // 8 milliseconds minimized duration while maintaining dot brightness.
  digitalWrite(laserPin, LOW);
  delay(2);  // because 8 + 2 = 10, and 10 is a nice number.
}

// play a noise, reload blaster, and wait for release of button
void reload() {
  // sounds start at 400 Hz and move up by 50, then 100, then 200, and so on.
  tone(buzzerPin, 400, 50);
  delay(50);
  tone(buzzerPin, 550, 50);
  delay(50);
  tone(buzzerPin, 750, 50);
  delay(50);
  tone(buzzerPin, 1150, 50);
  delay(50);
  tone(buzzerPin, 1950, 50);
  delay(50);
  roundNumber = 1;
  
  while (true) {
    if (digitalRead(buttonPin) == LOW) {
      delay(100);  // debounces button input and allows for momentary release of button
      if (digitalRead(buttonPin) == LOW) {
        break;
      }
    }
  }
}

// read the value of buttonPin, then initiate reloading, switch the gun mode, or do nothing
void checkButton() {
  if (digitalRead(buttonPin) == HIGH) {
    int long pressTime = millis();
    while (true) {
      if (digitalRead(buttonPin) == LOW) {
        delay(100); // debounces button input and allows for momentary release of button
        if (digitalRead(buttonPin) == LOW) {
          switchingModes = true;
          break;
        }
      }
      
      // to reload, the user must hold the button for a minimum of 1 second (1000 milliseconds)
      if (((millis() - pressTime) >= 1000) && modeNumber != 5) {
        reload();
        break;
      }
    }
  }
}

// delay, and check for button input between rounds
void delayBetweenRounds(int totalDelay) {
  int long delayStartTime = millis();
  
  // The loop will break if the user wants to reload or switch modes.
  while (((millis() - delayStartTime) < totalDelay) && (switchingModes == false) && (roundNumber != 1)) {
    checkButton();
  }
}

// semi-automatic pistol mode
void pistol() {
  modeNumber = 1;
  roundNumber = 1;
  while (!switchingModes) {
    if ((digitalRead(triggerPin) == HIGH) && (roundNumber <= 16)) {
      shootLaser();
      roundNumber++;
      while (true) {
        if (digitalRead(triggerPin) == LOW) {
          delayBetweenRounds(50); // debounces button input and allows for momentary release of button
          if (digitalRead(triggerPin) == LOW) {
            break;
          }
        }
      }
    }
    checkButton();
  }
  switchingModes = false; // resets switchingModes for the next mode
}

// assault rifle mode
void AR() {
  modeNumber = 2;
  roundNumber = 1;
  while (!switchingModes) {
    if ((digitalRead(triggerPin) == HIGH) && (roundNumber <= 30)) {
      shootLaser();
      roundNumber++;
      delayBetweenRounds(170); // time based on legendary assault rifle in Fortnite
    }
    checkButton();
  }
  switchingModes = false;  // resets switchingModes for the next mode
}

// burst assault rifle mode
void burst() {
  modeNumber = 3;
  roundNumber = 1;
  while (!switchingModes) {
    if ((digitalRead(triggerPin) == HIGH) && (roundNumber <= 30)) {
      shootLaser();
      roundNumber++; // so that delayBetweenRounds() works properly
      delayBetweenRounds(90); // time based on legendary burst assault rifle in Fortnite
      shootLaser();
      delayBetweenRounds(90);
      shootLaser();
      // fires a burst of 3 shots, even if user releases trigger before end of burst
      roundNumber = roundNumber + 2;
      delayBetweenRounds(390); // time based on legendary burst assault rifle in Fortnite
    }
    checkButton();
  }
  switchingModes = false; // resets switchingModes for next mode
}

// rapid fire/submachine gun mode
void SMG() {
  modeNumber = 4;
  roundNumber = 1;
  while (!switchingModes) {
    if ((digitalRead(triggerPin) == HIGH) && (roundNumber <= 30)) {
      shootLaser();
      roundNumber++;
      delayBetweenRounds(70); // time based on legendary submachine gun in Fortnite
    }
    checkButton();
  }
  switchingModes = false; // resets switchingModes for the next mode
}

// semi-auto pistol mode with no sound + no reloading
void pistolNightMode() {
  modeNumber = 5;
  roundNumber = 2;
  while (!switchingModes) {
    if (digitalRead(triggerPin) == HIGH) {
      digitalWrite(laserPin, HIGH);
      delay(8);  // 8 milliseconds minimized duration while maintaining dot brightness.
      digitalWrite(laserPin, LOW);
      delay(2);  // because 8 + 2 = 10, and 10 is a nice number.
      while (true) {
        if (digitalRead(triggerPin) == LOW) {
          delayBetweenRounds(50); // debounces button input and allows for momentary release of button
          if (digitalRead(triggerPin) == LOW) {
            break;
          }
        }
      }
    }
    checkButton();
  }
  switchingModes = false; // resets switchingModes for the next mode
}
