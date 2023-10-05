#include "ControlScreen.h"
#include "../bmp/home.h"
#include "../bmp/reset.h"
#include "../bmp/unlock.h"
#include "../bmp/esc.h"

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

    receiver->onStatusReceived([this](GrblStatusParser::GrblStatus status)
                               { drawStatus(status); });
}

void ControlScreen::hideHook()
{
    receiver->onStatusReceived(nullptr);
    statusValues.clear();
}

void ControlScreen::drawStatus(const GrblStatusParser::GrblStatus status)
{
    tft->setTextColor(ST7735_GREY);
    if (valueChanged("x", status.x))
        printAxisValue(0, 'X', status.x);
    if (valueChanged("y", status.y))
        printAxisValue(tft->width() / 3, 'Y', status.y);
    if (valueChanged("z", status.z))
        printAxisValue(tft->width() / 3 * 2, 'Z', status.z);

    tft->setTextColor(ST7735_BLACK);
#define WIDTH_STATE (3 * 6 + 1)
    String state = receiver->toString(status.state).substring(0, 3);
    if (valueChanged("state", state))
    {
        tft->fillRect(0, tft->height() - 10, WIDTH_STATE, 10, getStateColor(status.state));
        tft->setCursor(1, tft->height() - 9);
        tft->print(state);
    }

#define WIDTH_SPACE 1
#define WIDTH_ENDSTOP (6 + 1)
    if (valueChanged("xe", String(status.xEndstop)))
        drawEndstopState(WIDTH_STATE + WIDTH_SPACE, 'X', status.xEndstop);
    if (valueChanged("ye", String(status.yEndstop)))
        drawEndstopState(WIDTH_STATE + WIDTH_SPACE * 2 + WIDTH_ENDSTOP, 'Y', status.yEndstop);
    if (valueChanged("ze", String(status.zEndstop)))
        drawEndstopState(WIDTH_STATE + WIDTH_SPACE * 3 + WIDTH_ENDSTOP * 2, 'Z', status.zEndstop);
    if (valueChanged("p", String(status.probe)))
        drawEndstopState(WIDTH_STATE + WIDTH_SPACE * 4 + WIDTH_ENDSTOP * 3, 'P', status.probe);
}

bool ControlScreen::valueChanged(const String key, const String value)
{
    auto a = statusValues.find(key);
    if (a == statusValues.end())
    {
        statusValues.insert(std::pair<String, String>(key, value));
        return true;
    }
    else if (a->second == value)
    {
        return false;
    }
    else
    {
        a->second = value;
        return true;
    }
}

void ControlScreen::printAxisValue(const uint8_t x, const char axis, const String value)
{
    tft->fillRect(x, 16, tft->width() / 3, 10, ST7735_BLACK);
    tft->setCursor(x + 1, 17);
    tft->print(axis);
    if (value.charAt(0) != '-')
        tft->print('+');
    tft->print(value);
    tft->print(" ");
}

void ControlScreen::drawEndstopState(const uint8_t x, const char axis, const bool hit)
{
    tft->fillRect(x, tft->height() - 10, 6 + 1, 10, hit ? ST7735_RED : ST7735_GREEN);
    tft->setCursor(x + 1, tft->height() - 9);
    tft->print(axis);
}

uint16_t ControlScreen::getStateColor(const GrblStatusParser::GrblState state)
{
    switch (state)
    {
    case GrblStatusParser::Alarm:
    case GrblStatusParser::Unknown:
        return ST7735_RED;

    case GrblStatusParser::Door:
    case GrblStatusParser::Hold:
    case GrblStatusParser::Check:
    case GrblStatusParser::Sleep:
        return ST7735_ORANGE;

    default:
        return ST7735_GREEN;
    }
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
    else if (mode == Move)
    {
        buttons->onButton(VevorButtons::BT_BUTTON_X_UP, VevorButtons::Press, [this]()
                          { move(GrblSender::X, true); });
        buttons->onButton(VevorButtons::BT_BUTTON_X_DOWN, VevorButtons::Press, [this]()
                          { move(GrblSender::X, false); });
        buttons->onButton(VevorButtons::BT_BUTTON_Y_UP, VevorButtons::Press, [this]()
                          { move(GrblSender::Y, true); });
        buttons->onButton(VevorButtons::BT_BUTTON_Y_DOWN, VevorButtons::Press, [this]()
                          { move(GrblSender::Y, false); });
        buttons->onButton(VevorButtons::BT_BUTTON_Z_UP, VevorButtons::LongPress, [this]()
                          { sender->sendUnlock(); });
        buttons->onButton(VevorButtons::BT_BUTTON_Z_UP, VevorButtons::Press, [this]()
                          { move(GrblSender::Z, true); });
        buttons->onButton(VevorButtons::BT_BUTTON_Z_DOWN, VevorButtons::LongPress, [this]()
                          { sender->sendHome(); });
        buttons->onButton(VevorButtons::BT_BUTTON_Z_DOWN, VevorButtons::Press, [this]()
                          { move(GrblSender::Z, false); });
        buttons->onButton(VevorButtons::BT_BUTTON_SET, VevorButtons::LongPress, [this]()
                          { sender->sendReset(); });
    }
}

void ControlScreen::move(GrblSender::Axis axis, bool positive)
{
    MoveDistance distance = currentMoveDistance;
    if (axis == GrblSender::Z && distance > Ten)
        distance = Ten;
    sender->sendJog(axis, toFloat(distance) * (positive ? 1 : -1), config->getFeedRate());
}

void ControlScreen::changeSpeed(int8_t change)
{
    currentSpeed += change;
    if (currentSpeed > 100)
        currentSpeed = 100;
    if (currentSpeed < 0)
        currentSpeed = 0;

    if (currentSpeed == 0)
        sender->sendSpindelStop();
    else
        sender->sendSpindleSpeed(currentSpeed);

    redraw(false, currentMode, true);
}

void ControlScreen::switchMoveDistance()
{
    if (currentMoveDistance == Fifty)
        currentMoveDistance = Point1;
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
    drawButton(8, 30, ST7735_BLUE, mode == Move ? "Z+" : "100", VevorButtons::BT_BUTTON_Z_UP, forceDraw);
    drawButton(8, 90, ST7735_BLUE, mode == Move ? "Z-" : "Off", VevorButtons::BT_BUTTON_Z_DOWN, forceDraw);
    drawButton(93, 30, ST7735_BLUE, mode == Move ? "Y+" : "+10", VevorButtons::BT_BUTTON_Y_UP, forceDraw);
    drawButton(63, 60, ST7735_BLUE, mode == Move ? "X-" : "-1", VevorButtons::BT_BUTTON_X_DOWN, forceDraw);
    // Spindle
    drawButton(93, 60, ST7735_RED, mode == Move ? "Spd" : "Mov", VevorButtons::BT_BUTTON_SET, forceDraw);
    drawButton(123, 60, ST7735_BLUE, mode == Move ? "X+" : "+1", VevorButtons::BT_BUTTON_X_UP, forceDraw);
    drawButton(93, 90, ST7735_BLUE, mode == Move ? "Y-" : "-10", VevorButtons::BT_BUTTON_Y_DOWN, forceDraw);
    // Step
    drawButton(123, 90, ST7735_RED, toString(currentMoveDistance), VevorButtons::BT_BUTTON_ESC, forceDraw || forceStatusDraw);

    if (forceDraw)
    {
        if (mode == Move)
        {
            tft->drawRGBBitmap(116, 57, image_data_reset, 8, 10);
            tft->drawRGBBitmap(33, 30, image_data_unlock, 8, 9);
            tft->drawRGBBitmap(33, 90, image_data_home, 7, 8);
            tft->drawRGBBitmap(146, 90, image_data_esc, 11, 5);
        }
        else
        {
            tft->fillRect(116, 57, 8, 10, ST7735_BLACK);
            tft->fillRect(33, 30, 8, 9, ST7735_BLACK);
            tft->fillRect(33, 90, 7, 8, ST7735_BLACK);
            tft->fillRect(146, 90, 11, 5, ST7735_BLACK);
        }
    }
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

String ControlScreen::toString(MoveDistance moveDistance)
{
#define COMMAND_MOVE_DISTANCE_STRING(ENUM, STRING, FLOAT) \
    if (moveDistance == ENUM)                             \
        return STRING;
    FOREACH_MOVE_DISTANCE(COMMAND_MOVE_DISTANCE_STRING)
    return "??";
}

float_t ControlScreen::toFloat(MoveDistance moveDistance)
{
#define COMMAND_MOVE_DISTANCE_FLOAT(ENUM, STRING, FLOAT) \
    if (moveDistance == ENUM)                            \
        return FLOAT;
    FOREACH_MOVE_DISTANCE(COMMAND_MOVE_DISTANCE_FLOAT)
    return 0.0f;
}