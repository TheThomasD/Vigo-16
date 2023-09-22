#pragma once

#include "../config/VevorConfig.h"
#include "../tft/VevorST7735.h"
#include <WiFi.h>

class VevorWifi
{
public:
    VevorWifi(VevorST7735 *tft);
    void startWifi(VevorConfig* config);

protected:
    void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);

private:
    VevorST7735 *tft;
};