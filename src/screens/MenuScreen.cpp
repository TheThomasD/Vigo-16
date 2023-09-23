#include "MenuScreen.h"

#define VEVOR_YELLOW 0xfea5

void MenuScreen::showHook()
{
    redraw();

    buttons->onButton(VevorButtons::BT_BUTTON_Y_UP, VevorButtons::Press, [this](){changeMenuEntry(true);});
    buttons->onButton(VevorButtons::BT_BUTTON_Y_DOWN, VevorButtons::Press, [this](){changeMenuEntry(false);});
}

void MenuScreen::redraw()
{
    tft->clear();

    createMenuItem(20, "Control", selectedEntry == Control);
    createMenuItem(45, "File", selectedEntry == File);
    createMenuItem(70, "Settings", selectedEntry == Settings);
    createMenuItem(95, "Info", selectedEntry == Info);
}

void MenuScreen::changeMenuEntry(bool up)
{
    if (up && selectedEntry == Control || !up && selectedEntry == Info)
        return;

    selectedEntry = (MenuEntry)(selectedEntry + (up ? -1 : 1));
    redraw();
}

void MenuScreen::hideHook()
{
}

void MenuScreen::createMenuItem(uint8_t y, String caption, bool selected)
{
#define ITEM_HEIGHT 20
#define ITEM_WIDTH 140
#define TEXT_HEIGHT 8
#define TEXT_WIDTH 6

    const uint8_t x = (tft->width() - ITEM_WIDTH) / 2;
    if (selected)
        tft->fillRoundRect(x, y, ITEM_WIDTH, ITEM_HEIGHT, 12, VEVOR_YELLOW);
    else
        tft->drawRoundRect(x, y, ITEM_WIDTH, ITEM_HEIGHT, 12, VEVOR_YELLOW);
    tft->setTextColor(selected ? ST7735_BLACK : VEVOR_YELLOW);
    tft->setTextSize(1);
    tft->setCursor(x + (ITEM_WIDTH - caption.length() * TEXT_WIDTH) / 2, y + (ITEM_HEIGHT - TEXT_HEIGHT) / 2);
    tft->print(caption);
}