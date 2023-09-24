#pragma once
#include "../tft/VevorST7735.h"
#include "../buttons/VevorButtons.h"
#include <arduino-timer.h>

enum Screen
{
    SCREEN_MENU,
    SCREEN_CONTROL,
    SCREEN_FILE,
    SCREEN_SETTINGS,
    SCREEN_INFO
};

typedef std::function<void(Screen)> SwitchScreen;

class AScreen
{
public:
    AScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, SwitchScreen switchScreen);
    virtual void show() final;
    virtual void hide() final;
    virtual bool isActive() final;
    virtual void showHook() = 0;
    virtual void hideHook() = 0;

protected:
    VevorST7735 *tft;
    SwitchScreen switchScreen;
    VevorButtons *buttons;
    Timer<> *timer;

private:
    bool active = false;
};