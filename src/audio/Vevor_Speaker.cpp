#include "Vevor_Speaker.h"
#include <Arduino.h>

#define SPEAKER_PIN 21

static bool initialized = false;

void Vevor_Speaker::init()
{
  if (!initialized)
  {
    // setup ledc library for playing tones
    ledcSetup(0, 1000, 8);
    ledcAttachPin(SPEAKER_PIN, 0);
    setToneChannel(0);
    initialized = true;
  }
}

void Vevor_Speaker::playTone(unsigned int frequency, unsigned long duration)
{
  init();
  tone(SPEAKER_PIN, frequency, duration);
}