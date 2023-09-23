#pragma once
#include "AScreen.h"

class ControlScreen : public AScreen
{
public:
    ControlScreen(VevorST7735 *tft, VevorButtons *buttons, SwitchScreen switchScreen) : AScreen(tft, buttons, switchScreen){};
    void showHook();
    void hideHook();

protected:
    void drawButton(uint8_t x, uint8_t y, uint16_t color, String caption);
};