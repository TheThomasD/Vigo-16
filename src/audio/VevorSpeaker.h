#pragma once

#include "Pitches.h"

class VevorSpeaker
{
public:
    void playTone(unsigned int frequency, unsigned long duration);

private:
    VevorSpeaker();
};

VevorSpeaker Speaker;