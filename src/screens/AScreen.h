#pragma once
#include "../tft/VevorST7735.h"
#include "../buttons/VevorButtons.h"
#include <arduino-timer.h>

class AScreen
{
public:
    enum Screen
    {
        Menu,
        Control,
        File,
        Settings,
        Info
    };

    typedef std::function<void(Screen)> SwitchScreenCb;

    AScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, SwitchScreenCb switchScreenCb);
    virtual void show() final;
    virtual void hide() final;
    virtual bool isActive() final;
    virtual void showHook() = 0;
    virtual void hideHook(){};

protected:
    VevorST7735 *tft;
    SwitchScreenCb switchScreenCb;
    VevorButtons *buttons;
    Timer<> *timer;

private:
    bool active = false;
};