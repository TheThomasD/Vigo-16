#pragma once
#include "AScreen.h"

class SettingsScreen : public AScreen
{
public:
    SettingsScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, VevorConfig *config, SwitchScreenCb switchScreenCb) : AScreen(tft, timer, buttons, config, switchScreenCb){};
    void showHook();

protected:
    enum SelectedSetting
    {
        BaudRate,
        FeedRate,
        Save,
        Cancel
    };

    void redraw(bool onlyValues);
    void switchSetting(bool up);
    void editSetting(bool left);
    void editBaudRate(bool left);
    void editFeedRate(bool left);
    void action();

    SelectedSetting currentSetting = BaudRate;
};