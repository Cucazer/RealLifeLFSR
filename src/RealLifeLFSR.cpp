#include <Arduino.h>

int automaticModePin = 5;
int speedPin = A0;
int stepPin = 2;
int setPin = 4;

int latchPin = 11;      // (11) ST_CP [RCK] on 74HC595
int clockPin = 9;      // (9) SH_CP [SCK] on 74HC595
int dataPin = 12;     // (12) DS [S1] on 74HC595
int invertedResetPin = 7;

uint8_t initialSeed = 0xA5;

int prevStepPinValue;
int curStepPinValue;

void step(bool setData = false)
{
  digitalWrite(latchPin, LOW);

  if (setData)
  {
    digitalWrite(dataPin, HIGH);
  }
  
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);

  digitalWrite(dataPin, LOW);

  digitalWrite(latchPin, HIGH);
}

void setup() 
{
  pinMode(automaticModePin, INPUT_PULLUP);
  pinMode(stepPin, INPUT_PULLUP);
  pinMode(setPin, INPUT_PULLUP);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(invertedResetPin, OUTPUT);

  // reset shift register
  digitalWrite(invertedResetPin, LOW);
  step();
  digitalWrite(invertedResetPin, HIGH);

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, initialSeed);
  digitalWrite(latchPin, HIGH);

  digitalWrite(dataPin, LOW);

  prevStepPinValue = digitalRead(stepPin); // ensure normal feedback

  Serial.begin(9600);
}

void loop() 
{
  curStepPinValue = digitalRead(stepPin);

  if (!digitalRead(automaticModePin))
  {
    step(!digitalRead(setPin));
    //Serial.println(analogRead(speedPin));
    delay(analogRead(speedPin));
  } else if (prevStepPinValue == HIGH && curStepPinValue == LOW)
  {
    step(!digitalRead(setPin));
  }
  
  prevStepPinValue = curStepPinValue;
}
