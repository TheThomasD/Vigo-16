#pragma once

#include "../config/VevorConfig.h"
#include "../tft/VevorST7735.h"
#include <WiFi.h>
#include "../screens/VevorScreens.h"
#include <arduino-timer.h>

class VevorWifi
{
public:
    typedef std::function<void(String)> OnClientMessageCb;
    VevorWifi(VevorST7735 *tft);
    void startWifi(VevorConfig *config, Timer<> *timer, VevorScreens *screens);
    void onClientMessage(const OnClientMessageCb callback);
    void sendToClient(const String message);

protected:
    void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
    void handleWiFiServer();

private:
    VevorST7735 *tft;
    WiFiServer *server;
    WiFiClient client;
    OnClientMessageCb clientMessageCb;
};