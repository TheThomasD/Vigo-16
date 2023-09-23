#pragma once
#include "../tft/VevorST7735.h"

class AScreen
{
public:
    AScreen(VevorST7735 *tft);
    virtual void show() final;
    virtual void hide() final;
    virtual bool isActive() final;
    virtual void showHook() = 0;
    virtual void hideHook() = 0;

protected:
    VevorST7735 *tft;

private:
    bool active = false;
};