#include "AScreen.h"

AScreen::AScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, SwitchScreenCb switchScreenCb)
{
    this->tft = tft;
    this->timer = timer;
    this->buttons = buttons;
    this->switchScreenCb = switchScreenCb;
}

void AScreen::show()
{
    active = true;
    tft->clear();
    showHook();
}

void AScreen::hide()
{
    buttons->clearAll();
    hideHook();
    active = false;
}

bool AScreen::isActive()
{
    return active;
}