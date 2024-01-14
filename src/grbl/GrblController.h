#pragma once

#include <ESPAsyncWebServer.h>
#include "../config/VevorConfig.h"
#include "../tft/VevorST7735.h"
#include <arduino-timer.h>
#include "GrblSender.h"
#include "GrblReceiver.h"
#include "../wifi/VevorWifi.h"

class GrblController
{
public:
    GrblController(HardwareSerial *serial, VevorConfig *config, VevorST7735 *tft, Timer<> *timer, AsyncWebSocket *webSocket)
    {
        this->serial = serial;
        this->config = config;
        this->tft = tft;
        this->timer = timer;

        sender = new GrblSender(serial);
        receiver = new GrblReceiver(serial, sender, tft, webSocket);
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