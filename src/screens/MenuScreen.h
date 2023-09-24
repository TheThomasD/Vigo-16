#pragma once
#include "AScreen.h"

class MenuScreen : public AScreen
{
public:
    MenuScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, SwitchScreenCb switchScreenCb) : AScreen(tft, timer, buttons, switchScreenCb){};
    void showHook();

protected:
    void createMenuItem(uint8_t y, String caption, bool selected);
    void changeMenuEntry(bool up);
    void selectEntry();
    void redraw();

    enum MenuEntry
    {
        Control,
        File,
        Settings,
        Info
    };

    MenuEntry selectedEntry = Control;
};