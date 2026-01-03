#pragma once

#include "pitches.h"

class VevorSpeaker
{
public:
    VevorSpeaker();
    void playTone(unsigned int frequency, unsigned long duration);
};