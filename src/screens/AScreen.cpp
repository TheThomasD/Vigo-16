#include "AScreen.h"

AScreen::AScreen(VevorST7735 *tft)
{
    this->tft = tft;
}

void AScreen::show()
{
    active = true;
    tft->clear();
    showHook();
}

void AScreen::hide()
{
    hideHook();
    active = false;
}

bool AScreen::isActive()
{
    return active;
}