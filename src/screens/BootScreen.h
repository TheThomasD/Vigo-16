#pragma once
#include "AScreen.h"

class BootScreen : public AScreen
{
public:
    BootScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, SwitchScreenCb switchScreenCb);
    void showHook();
    void addStatusLine(String line);

private:
    String status[5];
};