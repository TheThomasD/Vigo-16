#pragma once
#include "AScreen.h"
#include "../grbl/GrblController.h"

class ControlScreen : public AScreen
{
public:
    ControlScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons,
                  VevorConfig *config, SwitchScreenCb switchScreenCb,
                  GrblController *grbl) : AScreen(tft, timer, buttons, config, switchScreenCb)
    {
        this->grbl = grbl;
    };
    void showHook();

protected:
    enum Mode
    {
        Move,
        Speed
    };
#define FOREACH_MOVE_DISTANCE(COMMAND) \
    COMMAND(Point5, ".5", 0.5f)        \
    COMMAND(One, "1", 1.0f)            \
    COMMAND(Five, "5", 5.0f)           \
    COMMAND(Ten, "10", 10.0f)          \
    COMMAND(Fifty, "50", 50.0f)        \
    COMMAND(Hundred, "100", 100.0f)
#define COMMAND_MOVE_DISTANCE_ENUM(ENUM, STRING, FLOAT) ENUM,
    enum MoveDistance
    {
        FOREACH_MOVE_DISTANCE(COMMAND_MOVE_DISTANCE_ENUM)
    };

    void redraw(bool forceDraw, Mode mode, bool forceStatusDraw);
    void drawButton(uint8_t x, uint8_t y, uint16_t color, String caption, VevorButtons::Button button, bool forceDraw);
    void registerButtons(Mode mode);
    void switchMode();
    String getMoveDistanceString(MoveDistance moveDistance);
    float_t getMoveDistance(MoveDistance moveDistance);
    void switchMoveDistance();
    void changeSpeed(int8_t change);

private:
    VevorButtons::ButtonStatus buttonStatus = VevorButtons::ButtonStatus();
    Mode currentMode = Move;
    MoveDistance currentMoveDistance = One;
    int16_t currentSpeed = 0;
    GrblController *grbl;
};