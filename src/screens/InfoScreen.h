#pragma once
#include "AScreen.h"

class InfoScreen : public AScreen
{
public:
    InfoScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, SwitchScreenCb switchScreenCb) : AScreen(tft, timer, buttons, switchScreenCb){};
    void showHook();
};