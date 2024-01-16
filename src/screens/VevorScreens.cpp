#include "VevorScreens.h"

VevorScreens::VevorScreens(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, VevorConfig *config, GrblSender *sender, GrblReceiver *receiver)
{
    AScreen::SwitchScreenCb func = [this](AScreen::Screen screen)
    {
        this->switchScreen(screen);
    };
    bootScreen = new BootScreen(tft, timer, buttons, config, func);
    controlScreen = new ControlScreen(tft, timer, buttons, config, func, sender, receiver);
    menuScreen = new MenuScreen(tft, timer, buttons, config, func);
    infoScreen = new InfoScreen(tft, timer, buttons, config, func);
    settingsScreen = new SettingsScreen(tft, timer, buttons, config, func);
    runScreen = new RunScreen(tft, timer, buttons, config, func);
    SetFileToRunCb setFileToRunCb = [this](File fileToRun)
    { runScreen->setFile(fileToRun); };
    filesScreen = new FilesScreen(tft, timer, buttons, config, func, setFileToRunCb);
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
    case AScreen::Screen::Files:
        switchToScreen(filesScreen);
        break;
    case AScreen::Screen::Settings:
        switchToScreen(settingsScreen);
        break;
    case AScreen::Screen::Run:
        switchToScreen(runScreen);
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
