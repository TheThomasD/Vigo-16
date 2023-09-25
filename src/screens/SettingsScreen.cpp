#include "SettingsScreen.h"

void SettingsScreen::showHook()
{
    currentSetting = BaudRate;
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

#define Y_DISTANCE (11 + 5)

void SettingsScreen::redraw(bool onlyValues)
{
    if (!onlyValues)
    {
        tft->clear();
        tft->setTitle("Settings");
    }

    tft->setTextSize(1);

    VevorConfig::BaudRate baudRate = config->getBaudRate();
    drawLine("Baud Rate:", String(config->toValue(baudRate)), 20, currentSetting == BaudRate, baudRate == VevorConfig::BR_9600, baudRate == VevorConfig::BR_921600, onlyValues);

    uint16_t feedRate = config->getFeedRate();
    drawLine("Feed Rate:", String(feedRate), 20 + Y_DISTANCE, currentSetting == FeedRate, feedRate <= 100, feedRate >= 6000, onlyValues);

    drawButton("Save", 20 + 2 * Y_DISTANCE, currentSetting == Save);
    drawButton("Cancel", 20 + 3 * Y_DISTANCE, currentSetting == Cancel);
}

#define X_OFFSET 12 * 6

void SettingsScreen::drawButton(String label, uint16_t y, bool selected)
{
    tft->fillRoundRect(
        X_OFFSET, y,
        tft->width() - X_OFFSET - 1, 12,
        6,
        selected ? ST7735_VEVOR_YELLOW : ST7735_BLACK);
    if (!selected)
        tft->drawRoundRect(
            X_OFFSET, y,
            tft->width() - X_OFFSET - 1, 12,
            6,
            ST7735_VEVOR_YELLOW);
    tft->setTextColor(selected ? ST7735_BLACK : ST7735_VEVOR_YELLOW);
    const uint16_t xOffset = X_OFFSET + (tft->width() - X_OFFSET - label.length() * 6) / 2;
    tft->setCursor(xOffset, y + 2);
    tft->print(label);
}

void SettingsScreen::drawLine(String label, String value, uint16_t y, bool editable, bool isMin, bool isMax, bool onlyValues)
{
    if (!onlyValues)
    {
        tft->setTextColor(ST7735_GREY);
        tft->setCursor(2, y + 2);
        tft->print(label);
    }

    tft->fillRect(
        X_OFFSET, y,
        tft->width() - X_OFFSET - 1, 12,
        editable ? ST7735_VEVOR_YELLOW : ST7735_BLACK);
    tft->setTextColor(editable ? ST7735_BLACK : ST7735_VEVOR_YELLOW);
    const uint16_t xOffset = X_OFFSET + (tft->width() - X_OFFSET - value.length() * 6) / 2;
    tft->setCursor(xOffset, y + 2);
    tft->print(value);

    if (editable)
    {
        if (!isMin)
            tft->fillTriangle(
                X_OFFSET + 2, y + 5,
                X_OFFSET + 6, y + 1,
                X_OFFSET + 6, y + 10, ST7735_BLACK);
        if (!isMax)
            tft->fillTriangle(
                tft->width() - 4, y + 5,
                tft->width() - 8, y + 1,
                tft->width() - 8, y + 10, ST7735_BLACK);
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
    if (left && config->getFeedRate() <= 100 || !left && config->getFeedRate() >= 6000)
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