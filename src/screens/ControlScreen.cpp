#include "ControlScreen.h"

void ControlScreen::showHook()
{
    tft->clear();

    tft->setTextColor(ST7735_WHITE);
    tft->setTextSize(1); // 6x8

    drawButton(10, 20, ST7735_BLUE, "Z+");
    drawButton(10, 80, ST7735_BLUE, "Z-");
    drawButton(95, 20, ST7735_BLUE, "Y+");
    drawButton(65, 50, ST7735_BLUE, "X-");
    // Spindle
    drawButton(95, 50, ST7735_RED, "SET");
    drawButton(125, 50, ST7735_BLUE, "X+");
    drawButton(95, 80, ST7735_BLUE, "Y-");
    // Step
    drawButton(125, 80, ST7735_RED, "ESC");
}

void ControlScreen::hideHook()
{
}

void ControlScreen::drawButton(uint8_t x, uint8_t y, uint16_t color, String caption)
{
#define BUTTON_WIDTH 24
#define BUTTON_HEIGHT 24
#define TEXT_START_Y (BUTTON_HEIGHT - 8) / 2

    tft->fillRoundRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, 12, color);
    tft->setCursor(x + (BUTTON_WIDTH - caption.length() * 6) / 2, y + TEXT_START_Y + (color == ST7735_BLUE ? 1 : 0));
    tft->print(caption);
}