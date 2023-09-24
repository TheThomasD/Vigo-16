#pragma once
#include "AScreen.h"

class BootScreen : public AScreen
{
public:
    BootScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, VevorConfig *config, SwitchScreenCb switchScreenCb) : AScreen(tft, timer, buttons, config, switchScreenCb)
    {
        for (uint8_t i = 0; i < 5; i++)
            status[i] = "";
    };
    void showHook();
    void addStatusLine(String line);

private:
    String status[5];
};