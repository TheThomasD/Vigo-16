#include "InfoScreen.h"
#include <WiFi.h>

void InfoScreen::showHook()
{
    redraw(false);

    buttons->onButton(VevorButtons::BT_BUTTON_ESC, VevorButtons::LongPress, [this]()
                      { switchScreenCb(AScreen::Menu); });

    timer->every(
        3000, [](void *infoScreen)
        {
                        InfoScreen *is = (InfoScreen *) infoScreen;
                        if (is->isActive()) {
                            is->redraw(true);
                        }
                        return is->isActive(); },
        this);
}

#define Y_DISTANCE 4
#define X_OFFSET 10 * 6

void InfoScreen::redraw(bool onlyValues)
{
    if (!onlyValues)
    {
        tft->clear();
        tft->setTitle("Info");
    }

    tft->setTextSize(1);
    tft->setCursor(1, 20 - Y_DISTANCE);

    printLine("AP:", WiFi.softAPSSID(), onlyValues);
    printLine("AP PW:", config->getApPassword(), onlyValues);
    printLine("AP IP:", WiFi.softAPIP().toString(), onlyValues);
    printLine("AP #Con:", String(WiFi.softAPgetStationNum()), onlyValues);
    printLine("STA:", WiFi.SSID(), onlyValues);
    printLine("STA stat:", getWifiStatus(), onlyValues);
    printLine("STA IP:", WiFi.localIP().toString(), onlyValues);
    printLine("Host:", WiFi.getHostname(), onlyValues);
    printLine("mDNS:", String(WiFi.getHostname()) + ".local", onlyValues);
}

void InfoScreen::printLine(String label, String value, bool onlyValues)
{
    tft->setCursor(2, tft->getCursorY() + Y_DISTANCE);
    if (!onlyValues)
    {
        tft->setTextColor(0x7bcf); // grey
        tft->print(label);
    }
    if (valueChanged(label, value))
    {
        tft->fillRect(X_OFFSET, tft->getCursorY(), tft->width() - X_OFFSET, 8, ST7735_BLACK);
        tft->setCursor(X_OFFSET, tft->getCursorY());
        tft->setTextColor(ST7735_WHITE);
        tft->println(value);
    }
    else
    {
        tft->println();
    }
}

bool InfoScreen::valueChanged(String label, String value)
{
    auto a = values.find(label);
    if (a == values.end())
    {
        values.insert(std::pair<String, String>(label, value));
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

String InfoScreen::getWifiStatus()
{
    switch (WiFi.status())
    {
    case WL_CONNECTED:
        return "Connected";
    case WL_CONNECT_FAILED:
        return "Connect failed";
    case WL_CONNECTION_LOST:
        return "Connection lost";
    case WL_IDLE_STATUS:
        return "Idle";
    case WL_NO_SSID_AVAIL:
        return "No SSID";

    case WL_DISCONNECTED:
    default:
        return "Disconnected";
    }
}