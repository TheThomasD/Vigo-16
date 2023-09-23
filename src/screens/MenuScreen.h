#pragma once
#include "AScreen.h"

class MenuScreen : public AScreen
{
public:
    MenuScreen(VevorST7735 *tft, VevorButtons *buttons, SwitchScreen switchScreen) : AScreen(tft, buttons, switchScreen){};
    void showHook();
    void hideHook();

protected:
    void createMenuItem(uint8_t y, String caption, bool selected);
    void changeMenuEntry(bool up);
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