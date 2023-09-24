#include "BootScreen.h"
#include "../bmp/Vevor.h"

BootScreen::BootScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, SwitchScreen switchScreen) : AScreen(tft, timer, buttons, switchScreen)
{
    for (uint8_t i = 0; i < 5; i++)
        status[i] = "";
}

void BootScreen::showHook()
{
    tft->clear();
    tft->drawRGBBitmap(0, 17, image_data_Vevor, 160, 60);
}

void BootScreen::hideHook()
{
}

void BootScreen::addStatusLine(String line)
{
    int16_t currentY = tft->height() - 5 * 9;
    tft->fillRect(0, currentY, tft->width(), 5 * 9, ST7735_BLACK);

    tft->setTextSize(1);
    tft->setTextWrap(false);
    tft->setTextColor(ST7735_WHITE);

    for (uint8_t i = 4; i > 0; i--)
    {
        status[i] = status[i - 1];
        tft->setCursor(1, currentY);
        tft->print(status[i]);
        currentY += 9;
    }
    if (line.length() > 26)
    {
        status[0] = line.substring(0, 25);
        addStatusLine(" " + line.substring(25));
    }
    else
    {
        status[0] = line;
        tft->setCursor(1, currentY);
        tft->print(line);
    }
}