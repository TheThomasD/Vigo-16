#pragma once
#include "AScreen.h"
#include "../grbl/GrblSender.h"
#include "../grbl/GrblReceiver.h"
#include <map>

class ControlScreen : public AScreen
{
public:
    ControlScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons,
                  VevorConfig *config, SwitchScreenCb switchScreenCb,
                  GrblSender *sender, GrblReceiver *receiver) : AScreen(tft, timer, buttons, config, switchScreenCb)
    {
        this->sender = sender;
        this->receiver = receiver;
    };
    void showHook();
    void hideHook();

protected:
    enum Mode
    {
        Move,
        Speed
    };
#define FOREACH_MOVE_DISTANCE(COMMAND) \
    COMMAND(Point1, ".1", 0.1f)        \
    COMMAND(Point5, ".5", 0.5f)        \
    COMMAND(One, "1", 1.0f)            \
    COMMAND(Five, "5", 5.0f)           \
    COMMAND(Ten, "10", 10.0f)          \
    COMMAND(Fifty, "50", 50.0f)
#define COMMAND_MOVE_DISTANCE_ENUM(ENUM, STRING, FLOAT) ENUM,
    enum MoveDistance
    {
        FOREACH_MOVE_DISTANCE(COMMAND_MOVE_DISTANCE_ENUM)
    };

    void redraw(bool forceDraw, Mode mode, bool forceStatusDraw);
    void drawButton(uint8_t x, uint8_t y, uint16_t color, String caption, VevorButtons::Button button, bool forceDraw);
    void registerButtons(Mode mode);
    void switchMode();
    String toString(MoveDistance moveDistance);
    float_t toFloat(MoveDistance moveDistance);
    void switchMoveDistance();
    void changeSpeed(int8_t change);
    void move(GrblSender::Axis axis, bool positive);
    void drawStatus(const GrblStatusParser::GrblStatus& status);
    uint16_t getStateColor(const GrblStatusParser::GrblState state);
    void drawEndstopState(const uint8_t x, const char axis, const bool hit);
    void printAxisValue(const uint8_t x, const char axis, const String value);
    bool valueChanged(const String key, const String value);

private:
    VevorButtons::ButtonStatus buttonStatus = VevorButtons::ButtonStatus();
    Mode currentMode = Move;
    MoveDistance currentMoveDistance = One;
    int16_t currentSpeed = 0;
    GrblSender *sender;
    GrblReceiver *receiver;
    std::map<String, String> statusValues;
};