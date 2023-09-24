#include "ControlScreen.h"

void ControlScreen::showHook()
{
    buttons->onButton(VevorButtons::BT_BUTTON_ESC, VevorButtons::LongPress, [this]()
                      { switchScreen(SCREEN_MENU); });

    redraw();
}

void ControlScreen::redraw()
{
    tft->clear();

    tft->setTextColor(ST7735_WHITE);
    tft->setTextSize(1); // 6x8

    drawButton(10, 20, ST7735_BLUE, "Z+", buttons->isPressed(VevorButtons::BT_BUTTON_Z_UP));
    drawButton(10, 80, ST7735_BLUE, "Z-", buttons->isPressed(VevorButtons::BT_BUTTON_Z_DOWN));
    drawButton(95, 20, ST7735_BLUE, "Y+", buttons->isPressed(VevorButtons::BT_BUTTON_Y_UP));
    drawButton(65, 50, ST7735_BLUE, "X-", buttons->isPressed(VevorButtons::BT_BUTTON_X_DOWN));
    // Spindle
    drawButton(95, 50, ST7735_RED, "SET", buttons->isPressed(VevorButtons::BT_BUTTON_SET));
    drawButton(125, 50, ST7735_BLUE, "X+", buttons->isPressed(VevorButtons::BT_BUTTON_X_UP));
    drawButton(95, 80, ST7735_BLUE, "Y-", buttons->isPressed(VevorButtons::BT_BUTTON_Y_DOWN));
    // Step
    drawButton(125, 80, ST7735_RED, "ESC", buttons->isPressed(VevorButtons::BT_BUTTON_ESC));
}

void ControlScreen::hideHook()
{
}

void ControlScreen::drawButton(uint8_t x, uint8_t y, uint16_t color, String caption, bool pressed)
{
#define BUTTON_WIDTH 24
#define BUTTON_HEIGHT 24
#define TEXT_START_Y (BUTTON_HEIGHT - 8) / 2

    if (pressed)
        tft->drawRoundRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, 12, color);
    else
        tft->fillRoundRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, 12, color);
    tft->setCursor(x + (BUTTON_WIDTH - caption.length() * 6) / 2, y + TEXT_START_Y + (color == ST7735_BLUE ? 1 : 0));
    tft->print(caption);
}