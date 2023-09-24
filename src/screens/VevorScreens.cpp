#include "VevorScreens.h"

VevorScreens::VevorScreens(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons)
{
    SwitchScreen func = [this](Screen screen)
    {
        this->switchScreen(screen);
    };
    bootScreen = new BootScreen(tft, timer, buttons, func);
    controlScreen = new ControlScreen(tft, timer, buttons, func);
    menuScreen = new MenuScreen(tft, timer, buttons, func);
}

void VevorScreens::switchScreen(Screen screen)
{
    switch (screen)
    {
    case SCREEN_MENU:
        showMenuScreen();
        break;
    case SCREEN_CONTROL:
        showControlScreen();
        break;
    case SCREEN_FILE:
        /* code */
        break;
    case SCREEN_SETTINGS:
        /* code */
        break;
    case SCREEN_INFO:
        /* code */
        break;

    default:
        break;
    }
}

void VevorScreens::showControlScreen()
{
    switchToScreen(controlScreen);
}

void VevorScreens::showBootScreen()
{
    switchToScreen(bootScreen);
}

void VevorScreens::showMenuScreen()
{
    switchToScreen(menuScreen);
}

void VevorScreens::switchToScreen(AScreen *screen)
{
    if (currentScreen != nullptr)
        currentScreen->hide();
    currentScreen = screen;
    screen->show();
}

void VevorScreens::addBootStatusLine(String line)
{
    if (bootScreen->isActive())
        bootScreen->addStatusLine(line);
}
