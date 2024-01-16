#pragma once
#include "../tft/VevorST7735.h"
#include "../buttons/VevorButtons.h"
#include "BootScreen.h"
#include "ControlScreen.h"
#include "MenuScreen.h"
#include "InfoScreen.h"
#include "SettingsScreen.h"
#include "FilesScreen.h"
#include "RunScreen.h"
#include <arduino-timer.h>
#include "../config/VevorConfig.h"
#include "../grbl/GrblSender.h"
#include "../grbl/GrblReceiver.h"

class VevorScreens
{
public:
    VevorScreens(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, VevorConfig *config, GrblSender *sender, GrblReceiver *receiver);
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
    SettingsScreen *settingsScreen;
    FilesScreen *filesScreen;
    RunScreen *runScreen;
    AScreen *currentScreen;
    Timer<> *timer;
};