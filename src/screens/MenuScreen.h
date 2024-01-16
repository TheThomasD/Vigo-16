#pragma once
#include "AScreen.h"

class MenuScreen : public AScreen
{
public:
    MenuScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, VevorConfig *config, SwitchScreenCb switchScreenCb) : AScreen(tft, timer, buttons, config, switchScreenCb){};
    void showHook();

protected:
    void createMenuItem(uint8_t y, String caption, bool selected);
    void changeMenuEntry(bool up);
    void selectEntry();
    void redraw();

    enum MenuEntry
    {
        Control,
        Files,
        Settings,
        Info
    };

    MenuEntry selectedEntry = Control;
};