// Touch-Tone Generator

// DTMF frequencies
uint16_t row_freqs[] = {697, 770, 852, 941};  // units Hz
uint16_t col_freqs[] = {1209, 1336, 1477};    // units Hz

struct key {
  char symbol;
  uint16_t row;
  uint16_t col;
};

struct key keypad[] = {
  {.symbol = '1', .row = 0, .col = 0},
  {.symbol = '2', .row = 0, .col = 1},
  {.symbol = '3', .row = 0, .col = 2},
  {.symbol = '4', .row = 1, .col = 0},
  {.symbol = '5', .row = 1, .col = 1},
  {.symbol = '6', .row = 1, .col = 2},
  {.symbol = '7', .row = 2, .col = 0},
  {.symbol = '8', .row = 2, .col = 1},
  {.symbol = '9', .row = 2, .col = 2},
  {.symbol = '*', .row = 3, .col = 0},
  {.symbol = '0', .row = 3, .col = 1},
  {.symbol = '#', .row = 3, .col = 2}
};

#define NUM_KEYS (sizeof keypad / sizeof keypad[0])

// Audio library things
#define CHANNEL_ROW      0
#define CHANNEL_COL      1
AudioSynthWaveformSine   sine_row;
AudioSynthWaveformSine   sine_col;
AudioMixer4              mixer1;
AudioOutputAnalog        dac1;
AudioConnection          patchCord1(sine_row, 0, mixer1, CHANNEL_ROW);
AudioConnection          patchCord2(sine_col, 0, mixer1, CHANNEL_COL);
AudioConnection          patchCord3(mixer1, dac1);

// Play DTMF signal
void play_tone(char key, uint16_t duration_ms) {
  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    if (key == keypad[i].symbol) {
      // Make sure channels are off
      mixer1.gain(CHANNEL_ROW, 0);
      mixer1.gain(CHANNEL_COL, 0);
      
      // Set row freq
      sine_row.amplitude(1);
      sine_row.frequency(row_freqs[keypad[i].row]);
      
      // Set col freq
      sine_col.amplitude(1);
      sine_col.frequency(col_freqs[keypad[i].col]);

      // Pulse on 
      mixer1.gain(CHANNEL_ROW, 1);
      mixer1.gain(CHANNEL_COL, 1);
      delay(duration_ms);   
      mixer1.gain(CHANNEL_ROW, 0);
      mixer1.gain(CHANNEL_COL, 0);
    }
  }
}
