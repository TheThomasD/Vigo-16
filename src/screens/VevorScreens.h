#pragma once
#include "../tft/VevorST7735.h"
#include "../buttons/VevorButtons.h"
#include "BootScreen.h"
#include "ControlScreen.h"
#include "MenuScreen.h"

class VevorScreens
{
public:
    VevorScreens(VevorST7735 *tft, VevorButtons *buttons);
    void showBootScreen();
    void showControlScreen();
    void showMenuScreen();
    void addBootStatusLine(String line);

private:
    void switchToScreen(AScreen *screen);
    void switchScreen(Screen screen);

    BootScreen *bootScreen;
    ControlScreen *controlScreen;
    MenuScreen *menuScreen;
    AScreen *currentScreen;
};