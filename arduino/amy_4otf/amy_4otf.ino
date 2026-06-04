// amy_4otf — 4-on-the-floor drum loop via AMY synthesizer on ESP32-S3 + PCM5102
//
// Board:     ESP32S3 Dev Module (esp32 by Espressif ≥3.3.8)
// Library:   AMY (search "AMY" in Arduino Library Manager)
// Partition: Huge APP (3MB No OTA/1MB SPIFFS)
// PSRAM:     OPI PSRAM (required for N16R8)
//
// PCM5102 wiring:
//   LRCLK → GPIO 4   (CONFIG_I2S_LRCLK)
//   BCLK  → GPIO 1   (CONFIG_I2S_BCLK)
//   DIN   → GPIO 2   (CONFIG_I2S_DOUT)
//   SCK   → GND      (PCM5102 self-clocking mode)
//   FMT   → GND      (I2S format)
//   XSMT  → 3V3      (unmute)
//   VCC   → 3V3, GND → GND

#include <AMY-Arduino.h>

// ── I2S / DAC pin assignments ────────────────────────────────────────────────
#define CONFIG_I2S_LRCLK  4
#define CONFIG_I2S_BCLK   1
#define CONFIG_I2S_DOUT   2

// ── Pattern: 4-on-the-floor at 120 BPM ──────────────────────────────────────
// 8-tick cycle; 1 tick = 1 eighth note = 125 ms at 120 BPM.
// GM drum channel 10 notes: 36 = kick, 38 = snare, 42 = closed HH, 46 = open HH

struct timed_note {
  float start_time;  // ticks (eighth notes)
  int   note;
  float velocity;
};

// Sorted ascending by start_time (required by the loop logic below).
static const timed_note notes[] = {
  { 0.0f, 36, 1.0f },  // beat 1 — kick
  { 0.0f, 42, 0.7f },  // beat 1 — closed HH
  { 1.0f, 42, 0.6f },  // & 1    — closed HH
  { 2.0f, 36, 1.0f },  // beat 2 — kick
  { 2.0f, 38, 0.9f },  // beat 2 — snare
  { 2.0f, 42, 0.7f },  // beat 2 — closed HH
  { 3.0f, 42, 0.6f },  // & 2    — closed HH
  { 4.0f, 36, 1.0f },  // beat 3 — kick
  { 4.0f, 42, 0.7f },  // beat 3 — closed HH
  { 5.0f, 42, 0.6f },  // & 3    — closed HH
  { 6.0f, 36, 1.0f },  // beat 4 — kick
  { 6.0f, 38, 0.9f },  // beat 4 — snare
  { 6.0f, 42, 0.7f },  // beat 4 — closed HH
  { 7.0f, 46, 0.8f },  // & 4    — open HH
};

static const float MILLIS_PER_TICK = 125.0f;  // 120 BPM, eighth-note grid
static const float CYCLE_LEN       = 8.0f;    // ticks per loop

static float base_tick     = 0.0f;
static int   note_tab_idx  = 0;
static const int note_tab_len = sizeof(notes) / sizeof(timed_note);

static void play_drum(int midi_note, float velocity) {
  amy_event e  = amy_default_event();
  e.synth      = 10;  // GM drum channel
  e.midi_note  = midi_note;
  e.velocity   = velocity;
  amy_add_event(&e);
}

// ── Arduino lifecycle ────────────────────────────────────────────────────────

void setup() {
  amy_config_t config = amy_default_config();

  config.audio   = AMY_AUDIO_IS_I2S;
  config.i2s_lrc  = CONFIG_I2S_LRCLK;
  config.i2s_bclk = CONFIG_I2S_BCLK;
  config.i2s_dout = CONFIG_I2S_DOUT;
  config.i2s_mclk = -1;  // PCM5102 self-clocking; no MCLK needed

  config.features.default_synths = 1;  // sets up drum kit on synth 10
  config.features.startup_bleep  = 1;  // audible boot confirmation

  amy_start(config);
}

void loop() {
  amy_update();  // must be called every iteration

  float tick_in_cycle = (float)millis() / MILLIS_PER_TICK - base_tick;

  if (tick_in_cycle >= CYCLE_LEN) {
    tick_in_cycle -= CYCLE_LEN;
    base_tick     += CYCLE_LEN;
    note_tab_idx   = 0;
  }

  while (note_tab_idx < note_tab_len
         && tick_in_cycle >= notes[note_tab_idx].start_time) {
    play_drum(notes[note_tab_idx].note, notes[note_tab_idx].velocity);
    note_tab_idx++;
  }
}
