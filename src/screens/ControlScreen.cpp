#include "ControlScreen.h"

void ControlScreen::showHook()
{
    tft->clear();
    currentMode = Move;
    redraw(true, currentMode, true);

    timer->every(
        50, [](void *controlScreen)
        { 
            ControlScreen *cs = (ControlScreen*) controlScreen;
            if (cs->isActive())
                cs->redraw(false, cs->currentMode, false);
            return cs->isActive(); },
        this);

    registerButtons(currentMode);

    grbl->getReceiver()->onStatusReceived([this](GrblStatusParser::GrblStatus status)
                                          {
                                              tft->setCursor(0, tft->height() - 9);
                                              tft->setTextColor(ST7735_WHITE);
                                              tft->print("Status " + grbl->getReceiver()->toString(status.state));
                                              tft->print(" X " + String(status.x)); });
}

void ControlScreen::switchMode()
{
    switch (currentMode)
    {
    case Move:
        currentMode = Speed;
        break;

    default:
        currentMode = Move;
        break;
    }
    registerButtons(currentMode);
    redraw(true, currentMode, false);
}

void ControlScreen::registerButtons(Mode mode)
{
    buttons->clearAll();
    buttons->onButton(VevorButtons::BT_BUTTON_ESC, VevorButtons::LongPress, [this]()
                      { switchScreenCb(AScreen::Menu); });
    buttons->onButton(VevorButtons::BT_BUTTON_ESC, VevorButtons::Press, [this]()
                      { switchMoveDistance(); });
    buttons->onButton(VevorButtons::BT_BUTTON_SET, VevorButtons::Press, [this]()
                      { switchMode(); });

    if (mode == Speed)
    {
        buttons->onButton(VevorButtons::BT_BUTTON_X_UP, VevorButtons::Press, [this]()
                          { changeSpeed(1); });
        buttons->onButton(VevorButtons::BT_BUTTON_X_DOWN, VevorButtons::Press, [this]()
                          { changeSpeed(-1); });
        buttons->onButton(VevorButtons::BT_BUTTON_Y_UP, VevorButtons::Press, [this]()
                          { changeSpeed(10); });
        buttons->onButton(VevorButtons::BT_BUTTON_Y_DOWN, VevorButtons::Press, [this]()
                          { changeSpeed(-10); });
        buttons->onButton(VevorButtons::BT_BUTTON_Z_UP, VevorButtons::Press, [this]()
                          { changeSpeed(100); });
        buttons->onButton(VevorButtons::BT_BUTTON_Z_DOWN, VevorButtons::Press, [this]()
                          { changeSpeed(-100); });
    }
}

void ControlScreen::changeSpeed(int8_t change)
{
    currentSpeed += change;
    if (currentSpeed > 100)
        currentSpeed = 100;
    if (currentSpeed < 0)
        currentSpeed = 0;
    redraw(false, currentMode, true);
}

void ControlScreen::switchMoveDistance()
{
    if (currentMoveDistance == Hundred)
        currentMoveDistance = Point5;
    else
        currentMoveDistance = (MoveDistance)(currentMoveDistance + 1);
    redraw(false, currentMode, true);
}

void ControlScreen::redraw(bool forceDraw, Mode mode, bool forceStatusDraw)
{
    if (forceDraw || forceStatusDraw)
    {
        String title = (String)(mode == Move ? "Move  " : "Speed ") + "- SPN:";
        if (currentSpeed == 0)
        {
            title = title + "off";
        }
        else
        {
            if (currentSpeed < 100)
                title = title + " ";
            if (currentSpeed < 10)
                title = title + " ";
            title = title + currentSpeed + "%";
        }
        tft->setTitle(title);
    }
    tft->setTextColor(ST7735_WHITE);
    tft->setTextSize(1); // 6x8
    drawButton(10, 30, ST7735_BLUE, mode == Move ? "Z+" : "100", VevorButtons::BT_BUTTON_Z_UP, forceDraw);
    drawButton(10, 90, ST7735_BLUE, mode == Move ? "Z-" : "Off", VevorButtons::BT_BUTTON_Z_DOWN, forceDraw);
    drawButton(95, 30, ST7735_BLUE, mode == Move ? "Y+" : "+10", VevorButtons::BT_BUTTON_Y_UP, forceDraw);
    drawButton(65, 60, ST7735_BLUE, mode == Move ? "X-" : "-1", VevorButtons::BT_BUTTON_X_DOWN, forceDraw);
    // Spindle
    drawButton(95, 60, ST7735_RED, mode == Move ? "Spd" : "Mov", VevorButtons::BT_BUTTON_SET, forceDraw);
    drawButton(125, 60, ST7735_BLUE, mode == Move ? "X+" : "+1", VevorButtons::BT_BUTTON_X_UP, forceDraw);
    drawButton(95, 90, ST7735_BLUE, mode == Move ? "Y-" : "-10", VevorButtons::BT_BUTTON_Y_DOWN, forceDraw);
    // Step
    drawButton(125, 90, ST7735_RED, getMoveDistanceString(currentMoveDistance), VevorButtons::BT_BUTTON_ESC, forceDraw || forceStatusDraw);
}

void ControlScreen::drawButton(uint8_t x, uint8_t y, uint16_t color, String caption, VevorButtons::Button button, bool forceDraw)
{
#define BUTTON_WIDTH 24
#define BUTTON_HEIGHT 24
#define TEXT_START_Y (BUTTON_HEIGHT - 8) / 2
    const bool pressed = buttons->isPressed(button);
    if (forceDraw || buttonStatus.stateChanged(button, pressed))
    {
        if (pressed)
        {
            tft->fillRoundRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, 12, ST7735_BLACK);
            tft->drawRoundRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, 12, color);
        }
        else
        {
            tft->fillRoundRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, 12, color);
        }
        tft->setCursor(x + (BUTTON_WIDTH - caption.length() * 6) / 2, y + TEXT_START_Y + (color == ST7735_BLUE ? 1 : 0));
        tft->print(caption);
    }
}

String ControlScreen::getMoveDistanceString(MoveDistance moveDistance)
{
#define COMMAND_MOVE_DISTANCE_STRING(ENUM, STRING, FLOAT) \
    if (moveDistance == ENUM)                             \
        return STRING;
    FOREACH_MOVE_DISTANCE(COMMAND_MOVE_DISTANCE_STRING)
    return "??";
}

float_t ControlScreen::getMoveDistance(MoveDistance moveDistance)
{
#define COMMAND_MOVE_DISTANCE_FLOAT(ENUM, STRING, FLOAT) \
    if (moveDistance == ENUM)                            \
        return FLOAT;
    FOREACH_MOVE_DISTANCE(COMMAND_MOVE_DISTANCE_FLOAT)
    return 0.0f;
}