#pragma once

#include "../config/VevorConfig.h"
#include "../tft/VevorST7735.h"
#include <arduino-timer.h>
#include "GrblSender.h"
#include "GrblReceiver.h"
#include "../wifi/VevorWifi.h"

class GrblController
{
public:
    GrblController(HardwareSerial *serial, VevorConfig *config, VevorST7735 *tft, Timer<> *timer)
    {
        this->serial = serial;
        this->config = config;
        this->tft = tft;
        this->timer = timer;

        receiver = new GrblReceiver(serial, tft);
        sender = new GrblSender(receiver);
    };

    void init(VevorWifi *wifi);
    GrblSender *getSender() { return sender; };
    GrblReceiver *getReceiver() { return receiver; };

protected:

private:
    HardwareSerial *serial;
    VevorConfig *config;
    VevorST7735 *tft;
    Timer<> *timer;

    GrblReceiver *receiver;
    GrblSender *sender;
};