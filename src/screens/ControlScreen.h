#pragma once
#include "AScreen.h"

class ControlScreen : public AScreen
{
public:
    ControlScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, SwitchScreen switchScreen) : AScreen(tft, timer, buttons, switchScreen){};
    void showHook();
    void hideHook();

protected:
    void redraw(bool forceDraw);
    void drawButton(uint8_t x, uint8_t y, uint16_t color, String caption, VevorButtons::Button button, bool forceDraw);
    VevorButtons::ButtonStatus buttonStatus = VevorButtons::ButtonStatus();
};