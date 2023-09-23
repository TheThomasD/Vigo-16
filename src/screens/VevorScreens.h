#pragma once
#include "../tft/VevorST7735.h"
#include "BootScreen.h"
#include "ControlScreen.h"

class VevorScreens
{
public:
    VevorScreens(VevorST7735 *tft);
    void showBootScreen();
    void showControlScreen();
    void addBootStatusLine(String line);

private:
    void switchToScreen(AScreen *screen);

    VevorST7735 *tft;
    BootScreen *bootScreen;
    ControlScreen *controlScreen;
    AScreen *currentScreen;
};