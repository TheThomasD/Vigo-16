#include "VevorScreens.h"

VevorScreens::VevorScreens(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons)
{
    AScreen::SwitchScreenCb func = [this](AScreen::Screen screen)
    {
        this->switchScreen(screen);
    };
    bootScreen = new BootScreen(tft, timer, buttons, func);
    controlScreen = new ControlScreen(tft, timer, buttons, func);
    menuScreen = new MenuScreen(tft, timer, buttons, func);
    infoScreen = new InfoScreen(tft, timer, buttons, func);
}

void VevorScreens::switchScreen(AScreen::Screen screen)
{
    switch (screen)
    {
    case AScreen::Screen::Menu:
        showMenuScreen();
        break;
    case AScreen::Screen::Control:
        switchToScreen(controlScreen);
        break;
    case AScreen::Screen::File:
        /* code */
        break;
    case AScreen::Screen::Settings:
        /* code */
        break;
    case AScreen::Screen::Info:
    default:
        switchToScreen(infoScreen);
        break;
    }
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
