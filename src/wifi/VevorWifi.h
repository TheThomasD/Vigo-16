#pragma once

#include "../config/VevorConfig.h"
#include "../tft/VevorST7735.h"
#include <WiFi.h>
#include "../screens/VevorScreens.h"
#include <arduino-timer.h>

class VevorWifi
{
public:
    typedef std::function<void(const char *buffer, const size_t size)> OnClientMessageCb;
    VevorWifi(VevorST7735 *tft);
    void startWifi(VevorConfig *config, Timer<> *timer, VevorScreens *screens);
    void onClientMessage(const OnClientMessageCb callback);
    void sendToClient(const char *buffer, const size_t size);

protected:
    void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
    void handleWiFiServer();

private:
    char messageBuffer[129];
    VevorST7735 *tft;
    WiFiServer *server;
    WiFiClient client;
    OnClientMessageCb clientMessageCb;
};