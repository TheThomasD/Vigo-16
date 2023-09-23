#include "VevorScreens.h"

VevorScreens::VevorScreens(VevorST7735 *tft)
{
    this->tft = tft;
    bootScreen = new BootScreen(tft);
    controlScreen = new ControlScreen(tft);
}

void VevorScreens::showControlScreen()
{
    switchToScreen(controlScreen);
}

void VevorScreens::showBootScreen()
{
    switchToScreen(bootScreen);
}

void VevorScreens::switchToScreen(AScreen *screen) {
    if (currentScreen != nullptr)
        currentScreen->hide();
    currentScreen = screen;
    screen->show();
}

void VevorScreens::addBootStatusLine(String line) {
    if (bootScreen->isActive())
        bootScreen->addStatusLine(line);
}
