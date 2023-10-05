#pragma once

#include "Pitches.h"

class VevorSpeaker
{
public:
    VevorSpeaker();
    void playTone(unsigned int frequency, unsigned long duration);
};