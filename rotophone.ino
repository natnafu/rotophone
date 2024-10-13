#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "touchtone.h"

#define DEBUG_MODE

#define DIAL_PIN      33
#define ROTO_DONE_MS  100
#define DEBOUNCE_MS   10

void read_dial() {
  int count = 0;
  if (digitalRead(DIAL_PIN)) {
    int timer_state_change_ms = millis();
    int lastState = 1;
    count++;
    
    while ((millis() - timer_state_change_ms) < ROTO_DONE_MS) {
      // Loop until done reading dial
      if ((millis() - timer_state_change_ms > DEBOUNCE_MS)) {
        // debounce
        if (digitalRead(DIAL_PIN) != lastState) {
          count++;
          lastState = !lastState;
          timer_state_change_ms = millis();
        }
      }
    }
    // Done counting pulses, convert to number
    count = (count / 2) % 10;
    play_tone(count + '0', 100 );
    Serial.println(count);
  }
  return ;
}

void setup() {
  Serial.begin(9600);
  
  // touchtone setup
  AudioMemory(20);
  mixer1.gain(CHANNEL_ROW, 0);
  mixer1.gain(CHANNEL_COL, 0);

  // dial setup
  pinMode(DIAL_PIN, INPUT);
}

char phone_number[10];
int p_index = 0;

void loop() {
  read_dial();
}
