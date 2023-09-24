#include "MenuScreen.h"

void MenuScreen::showHook()
{
    redraw();

    buttons->onButton(VevorButtons::BT_BUTTON_Y_UP, VevorButtons::Press, [this]()
                      { changeMenuEntry(true); });
    buttons->onButton(VevorButtons::BT_BUTTON_Y_DOWN, VevorButtons::Press, [this]()
                      { changeMenuEntry(false); });
    buttons->onButton(VevorButtons::BT_BUTTON_SET, VevorButtons::Press, [this]()
                      { selectEntry(); });
}

void MenuScreen::selectEntry()
{
    switch (selectedEntry)
    {
    case Control:
        switchScreenCb(AScreen::Control);
        break;
    case File:
        switchScreenCb(AScreen::File);
        break;
    case Settings:
        switchScreenCb(AScreen::Settings);
        break;

    case Info:
    default:
        switchScreenCb(AScreen::Info);
        break;
    }
}

void MenuScreen::redraw()
{
    tft->clear();
    tft->setTitle("Menu");

    createMenuItem(25, "Control", selectedEntry == Control);
    createMenuItem(50, "File", selectedEntry == File);
    createMenuItem(75, "Settings", selectedEntry == Settings);
    createMenuItem(100, "Info", selectedEntry == Info);
}

void MenuScreen::changeMenuEntry(bool up)
{
    if (up && selectedEntry == Control || !up && selectedEntry == Info)
        return;

    selectedEntry = (MenuEntry)(selectedEntry + (up ? -1 : 1));
    redraw();
}

void MenuScreen::createMenuItem(uint8_t y, String caption, bool selected)
{
#define ITEM_HEIGHT 20
#define ITEM_WIDTH 140
#define TEXT_HEIGHT 8
#define TEXT_WIDTH 6

    const uint8_t x = (tft->width() - ITEM_WIDTH) / 2;
    if (selected)
        tft->fillRoundRect(x, y, ITEM_WIDTH, ITEM_HEIGHT, 12, ST7735_VEVOR_YELLOW);
    else
        tft->drawRoundRect(x, y, ITEM_WIDTH, ITEM_HEIGHT, 12, ST7735_VEVOR_YELLOW);
    tft->setTextColor(selected ? ST7735_BLACK : ST7735_VEVOR_YELLOW);
    tft->setTextSize(1);
    tft->setCursor(x + (ITEM_WIDTH - caption.length() * TEXT_WIDTH) / 2, y + (ITEM_HEIGHT - TEXT_HEIGHT) / 2);
    tft->print(caption);
}