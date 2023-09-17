#include "VevorSpeaker.h"
#include <Arduino.h>

#define SPEAKER_PIN 21

bool VevorSpeaker::initialized = false;

void VevorSpeaker::init()
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

void VevorSpeaker::playTone(unsigned int frequency, unsigned long duration)
{
  init();
  tone(SPEAKER_PIN, frequency, duration);
}