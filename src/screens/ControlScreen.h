#pragma once
#include "AScreen.h"

class ControlScreen : public AScreen
{
public:
    ControlScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, SwitchScreen switchScreen) : AScreen(tft, timer, buttons, switchScreen){};
    void showHook();
    void hideHook();

protected:
    enum Mode
    {
        Move,
        Speed
    };
#define FOREACH_FEEDRATE(COMMAND) \
    COMMAND(Point5, ".5")         \
    COMMAND(One, "1")             \
    COMMAND(Five, "5")            \
    COMMAND(Ten, "10")            \
    COMMAND(Fifty, "50")          \
    COMMAND(Hundred, "100")
#define COMMAND_FEEDRATE_ENUM(ENUM, STRING) ENUM,
    enum Feedrate
    {
        FOREACH_FEEDRATE(COMMAND_FEEDRATE_ENUM)
    };

    void redraw(bool forceDraw, Mode mode, bool forceStatusDraw);
    void drawButton(uint8_t x, uint8_t y, uint16_t color, String caption, VevorButtons::Button button, bool forceDraw);
    void registerButtons(Mode mode);
    void switchMode();
    String getFeedrateString(Feedrate feedrate);
    void switchFeedrate();
    void changeSpeed(uint8_t change);

    VevorButtons::ButtonStatus buttonStatus = VevorButtons::ButtonStatus();
    Mode currentMode = Move;
    Feedrate currentFeedrate = One;
    uint8_t currentSpeed = 10;
};