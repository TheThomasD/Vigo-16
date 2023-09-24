#pragma once
#include "AScreen.h"
#include "../config/VevorConfig.h"
#include <map>

class InfoScreen : public AScreen
{
public:
    InfoScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, SwitchScreenCb switchScreenCb, VevorConfig *config) : AScreen(tft, timer, buttons, switchScreenCb) { this->config = config; };
    void showHook();

protected:
    void printLine(String label, String value, bool onlyValues);
    void redraw(bool onlyValues);
    String getWifiStatus();
    VevorConfig *config;
    std::map<String, String> values;
    bool valueChanged(String label, String value);
};