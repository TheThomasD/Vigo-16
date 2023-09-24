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
                      { switchScreen(SCREEN_MENU); });
    buttons->onButton(VevorButtons::BT_BUTTON_ESC, VevorButtons::Press, [this]()
                      { switchFeedrate(); });
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

void ControlScreen::changeSpeed(uint8_t change)
{
    currentSpeed += change;
    if (currentSpeed > 100)
        currentSpeed = 100;
    if (currentSpeed < 0)
        currentSpeed = 0;
}

void ControlScreen::switchFeedrate()
{
    if (currentFeedrate == Hundred)
        currentFeedrate = Point5;
    else
        currentFeedrate = (Feedrate)(currentFeedrate + 1);
    redraw(false, currentMode, true);
}

void ControlScreen::redraw(bool forceDraw, Mode mode, bool forceStatusDraw)
{
    tft->setTextColor(ST7735_WHITE);
    tft->setTextSize(1); // 6x8
    drawButton(10, 20, ST7735_BLUE, mode == Move ? "Z+" : "100", VevorButtons::BT_BUTTON_Z_UP, forceDraw);
    drawButton(10, 80, ST7735_BLUE, mode == Move ? "Z-" : "Off", VevorButtons::BT_BUTTON_Z_DOWN, forceDraw);
    drawButton(95, 20, ST7735_BLUE, mode == Move ? "Y+" : "+10", VevorButtons::BT_BUTTON_Y_UP, forceDraw);
    drawButton(65, 50, ST7735_BLUE, mode == Move ? "X-" : "-1", VevorButtons::BT_BUTTON_X_DOWN, forceDraw);
    // Spindle
    drawButton(95, 50, ST7735_RED, mode == Move ? "Spd" : "Mov", VevorButtons::BT_BUTTON_SET, forceDraw);
    drawButton(125, 50, ST7735_BLUE, mode == Move ? "X+" : "+1", VevorButtons::BT_BUTTON_X_UP, forceDraw);
    drawButton(95, 80, ST7735_BLUE, mode == Move ? "Y-" : "-10", VevorButtons::BT_BUTTON_Y_DOWN, forceDraw);
    // Step
    drawButton(125, 80, ST7735_RED, getFeedrateString(currentFeedrate), VevorButtons::BT_BUTTON_ESC, forceDraw || forceStatusDraw);
}

void ControlScreen::hideHook()
{
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

String ControlScreen::getFeedrateString(Feedrate feedrate)
{
#define COMMAND_FEEDRATRE_STRING(ENUM, STRING) \
    if (feedrate == ENUM)                      \
        return STRING;
    FOREACH_FEEDRATE(COMMAND_FEEDRATRE_STRING)
    return "??";
}