#pragma once
#include "../tft/VevorST7735.h"
#include "../buttons/VevorButtons.h"
#include <arduino-timer.h>
#include "../config/VevorConfig.h"

#define ST7735_GREY 0x7bcf
#define ST7735_VEVOR_YELLOW 0xfea5

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

    AScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, VevorConfig *config, SwitchScreenCb switchScreenCb)
    {
        this->tft = tft;
        this->timer = timer;
        this->buttons = buttons;
        this->config = config;
        this->switchScreenCb = switchScreenCb;
    };
    virtual void show() final;
    virtual void hide() final;
    virtual bool isActive() final;
    virtual void showHook() = 0;
    virtual void hideHook(){};

protected:
    VevorST7735 *tft;
    SwitchScreenCb switchScreenCb;
    VevorButtons *buttons;
    VevorConfig *config;
    Timer<> *timer;

private:
    bool active = false;
};