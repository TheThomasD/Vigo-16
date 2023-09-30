#pragma once

#include "GrblReceiver.h"

class GrblSender
{
public:
    enum Axis
    {
        X,
        Y,
        Z
    };

    GrblSender(GrblReceiver *receiver)
    {
        this->receiver = receiver;
        this->serial = receiver->serial;
    };
    void queryStatus();
    void sendHome();
    void sendUnlock();
    void sendReset();
    void sendJog(Axis axis, float units, uint16_t feedRate);
    void sendSpindleSpeed(uint8_t percent);
    void sendSpindelStop();

protected:
    char getAxisChar(Axis axis);

private:
    HardwareSerial *serial;
    GrblReceiver *receiver;
};