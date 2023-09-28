#pragma once

#include "GrblReceiver.h"

class GrblSender
{
public:
    GrblSender(GrblReceiver *receiver)
    {
        this->receiver = receiver;
        this->serial = receiver->serial;
    };
    void queryStatus();

private:
    HardwareSerial *serial;
    GrblReceiver *receiver;
};