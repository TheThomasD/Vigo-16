#include <Arduino.h>
#include "Vevor_Reset.h"

#define RESET_PIN 22

bool Vevor_Reset::initialized = false;

void Vevor_Reset::init() {
    if (!initialized) {
        pinMode(RESET_PIN, OUTPUT);
        initialized = true;
    }
}

void Vevor_Reset::reset()
{
    init();
    digitalWrite(RESET_PIN, 1);
    delay(1000);
    digitalWrite(RESET_PIN, 0);
}