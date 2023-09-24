#pragma once
#include "AScreen.h"
#include "../config/VevorConfig.h"
#include <map>

class InfoScreen : public AScreen
{
public:
    InfoScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, VevorConfig *config, SwitchScreenCb switchScreenCb) : AScreen(tft, timer, buttons, config, switchScreenCb) {};
    void showHook();

protected:
    void printLine(String label, String value, bool onlyValues);
    void redraw(bool onlyValues);
    String getWifiStatus();
    std::map<String, String> values;
    bool valueChanged(String label, String value);
};