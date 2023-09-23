#pragma once
#include "AScreen.h"

class BootScreen : public AScreen
{
public:
    BootScreen(VevorST7735 *tft, VevorButtons *buttons, SwitchScreen switchScreen);
    void showHook();
    void hideHook();
    void addStatusLine(String line);

private:
    String status[5];
};