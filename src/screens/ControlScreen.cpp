#include "ControlScreen.h"

void ControlScreen::showHook()
{
    buttons->onButton(VevorButtons::BT_BUTTON_ESC, VevorButtons::LongPress, [this]()
                      { switchScreen(SCREEN_MENU); });

    tft->clear();
    tft->setTextColor(ST7735_WHITE);
    tft->setTextSize(1); // 6x8
    redraw(true);

    timer->every(
        50, [](void *controlScreen)
        { 
            ControlScreen *cs = (ControlScreen*) controlScreen;
            if (cs->isActive())
                cs->redraw(false);
            return cs->isActive(); },
        this);
}

void ControlScreen::redraw(bool forceDraw)
{
    drawButton(10, 20, ST7735_BLUE, "Z+", VevorButtons::BT_BUTTON_Z_UP, forceDraw);
    drawButton(10, 80, ST7735_BLUE, "Z-", VevorButtons::BT_BUTTON_Z_DOWN, forceDraw);
    drawButton(95, 20, ST7735_BLUE, "Y+", VevorButtons::BT_BUTTON_Y_UP, forceDraw);
    drawButton(65, 50, ST7735_BLUE, "X-", VevorButtons::BT_BUTTON_X_DOWN, forceDraw);
    // Spindle
    drawButton(95, 50, ST7735_RED, "SET", VevorButtons::BT_BUTTON_SET, forceDraw);
    drawButton(125, 50, ST7735_BLUE, "X+", VevorButtons::BT_BUTTON_X_UP, forceDraw);
    drawButton(95, 80, ST7735_BLUE, "Y-", VevorButtons::BT_BUTTON_Y_DOWN, forceDraw);
    // Step
    drawButton(125, 80, ST7735_RED, "ESC", VevorButtons::BT_BUTTON_ESC, forceDraw);
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