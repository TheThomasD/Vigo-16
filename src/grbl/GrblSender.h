#pragma once

#include <Arduino.h>

class GrblSender
{
public:
    enum Axis
    {
        X,
        Y,
        Z
    };

    GrblSender(HardwareSerial *serial)
    {
        this->serial = serial;
    };
    void queryStatus();
    void sendHome();
    void sendUnlock();
    void sendReset();
    void sendProbe();
    void sendJog(Axis axis, float units, uint16_t feedRate);
    void sendSpindleSpeed(uint8_t percent);
    void sendSpindelStop();

protected:
    char getAxisChar(Axis axis);

private:
    HardwareSerial *serial;
};