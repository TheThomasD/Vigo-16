#pragma once
#include "../tft/VevorST7735.h"
#include "../buttons/VevorButtons.h"
#include "BootScreen.h"
#include "ControlScreen.h"
#include "MenuScreen.h"
#include "InfoScreen.h"
#include <arduino-timer.h>

class VevorScreens
{
public:
    VevorScreens(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons);
    void showBootScreen();
    void showMenuScreen();
    void addBootStatusLine(String line);

private:
    void switchToScreen(AScreen *screen);
    void switchScreen(AScreen::Screen screen);

    BootScreen *bootScreen;
    ControlScreen *controlScreen;
    MenuScreen *menuScreen;
    InfoScreen *infoScreen;
    AScreen *currentScreen;
    Timer<> *timer;
};