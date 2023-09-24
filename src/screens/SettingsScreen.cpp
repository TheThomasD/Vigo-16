#include "SettingsScreen.h"

void SettingsScreen::showHook()
{
    redraw(false);

    buttons->onButton(VevorButtons::BT_BUTTON_ESC, VevorButtons::LongPress, [this]()
                      {
                          config->load();
                          switchScreenCb(AScreen::Menu); });
    buttons->onButton(VevorButtons::BT_BUTTON_SET, VevorButtons::Press, [this]()
                      { action(); });
    buttons->onButton(VevorButtons::BT_BUTTON_X_DOWN, VevorButtons::Press, [this]()
                      { editSetting(true); });
    buttons->onButton(VevorButtons::BT_BUTTON_X_UP, VevorButtons::Press, [this]()
                      { editSetting(false); });
    buttons->onButton(VevorButtons::BT_BUTTON_Y_UP, VevorButtons::Press, [this]()
                      { switchSetting(true); });
    buttons->onButton(VevorButtons::BT_BUTTON_Z_UP, VevorButtons::Press, [this]()
                      { switchSetting(true); });
    buttons->onButton(VevorButtons::BT_BUTTON_Y_DOWN, VevorButtons::Press, [this]()
                      { switchSetting(false); });
    buttons->onButton(VevorButtons::BT_BUTTON_Z_DOWN, VevorButtons::Press, [this]()
                      { switchSetting(false); });
}

#define Y_DISTANCE 4
#define X_OFFSET 12 * 6

void SettingsScreen::redraw(bool onlyValues)
{
    if (!onlyValues)
    {
        tft->clear();
        tft->setTitle("Settings");
    }

    tft->setTextSize(1);
    tft->setTextColor(ST7735_GREY);
    tft->setCursor(2, 20);
    tft->print("Baud Rate:");

    tft->fillRect(X_OFFSET - 2, tft->getCursorY() - 2, tft->width() - X_OFFSET, 8 + 3, currentSetting == BaudRate ? ST7735_VEVOR_YELLOW : ST7735_BLACK);
    tft->setTextColor(currentSetting == BaudRate ? ST7735_BLACK : ST7735_VEVOR_YELLOW);
    tft->setCursor(X_OFFSET + 12, tft->getCursorY());
    tft->print(config->toValue(config->getBaudRate()));
    if (currentSetting == BaudRate) {
        tft->fillTriangle(X_OFFSET, tft->getCursorY() + 3, X_OFFSET + 7, tft->getCursorY() - 1, X_OFFSET + 7, tft->getCursorY() + 7, ST7735_BLACK);
        tft->fillTriangle(tft->width() - 4, tft->getCursorY() + 3, tft->width() - 11, tft->getCursorY() - 1, tft->width() - 11, tft->getCursorY() + 7, ST7735_BLACK);
    }

    if (currentSetting == FeedRate)
    {
    }
    else
    {
    }

    if (currentSetting == Save)
    {
    }
    else
    {
    }

    if (currentSetting == Cancel)
    {
    }
}

void SettingsScreen::editSetting(bool left)
{
    if (currentSetting == BaudRate)
        editBaudRate(left);
    else if (currentSetting == FeedRate)
        editFeedRate(left);
}

void SettingsScreen::editBaudRate(bool left)
{
    if (left && config->getBaudRate() == VevorConfig::BR_9600 || !left && config->getBaudRate() == VevorConfig::BR_921600)
        return;

    config->setBaudRate(VevorConfig::BaudRate(config->getBaudRate() + (left ? -1 : 1)));
    redraw(true);
}

void SettingsScreen::editFeedRate(bool left)
{
    if (left && config->getFeedRate() <= 100 || !left && config->getBaudRate() >= 6000)
        return;
    config->setFeedRate(config->getFeedRate() + (left ? -100 : 100));
    redraw(true);
}

void SettingsScreen::switchSetting(bool up)
{
    if (up && currentSetting == BaudRate || !up && currentSetting == Cancel)
        return;
    currentSetting = (SelectedSetting)(currentSetting + (up ? -1 : 1));
    redraw(true);
}

void SettingsScreen::action()
{
    switch (currentSetting)
    {
    case Save:
        config->save();
        switchScreenCb(AScreen::Menu);
        break;
    case Cancel:
        config->load();
        switchScreenCb(AScreen::Menu);
        break;

    case BaudRate:
    case FeedRate:
    default:
        break;
    }
}