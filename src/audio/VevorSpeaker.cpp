#include "VevorSpeaker.h"
#include <Arduino.h>

#define SPEAKER_PIN 21

VevorSpeaker::VevorSpeaker()
{
  // setup ledc library for playing tones
  ledcSetup(0, 1000, 8);
  ledcAttachPin(SPEAKER_PIN, 0);
  setToneChannel(0);
}

void VevorSpeaker::playTone(unsigned int frequency, unsigned long duration)
{
  tone(SPEAKER_PIN, frequency, duration);
}