#include "AScreen.h"

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