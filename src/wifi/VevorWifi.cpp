#include "VevorWifi.h"
#include "../log/Logger.h"
#include <ESPmDNS.h>

VevorWifi::VevorWifi(VevorST7735 *tft)
{
    this->tft = tft;
}

void VevorWifi::startWifi(VevorConfig *config, Timer<> *timer, VevorScreens *screens)
{
    screens->addBootStatusLine("Connecting Wifi...");
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.disconnect();
    WiFi.softAPdisconnect();

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
                 { onWiFiEvent(event, info); },
                 ARDUINO_EVENT_MAX);

    if (WiFi.setHostname(config->getHostName().c_str()))
    {
        log_println("HostName: " + config->getHostName());
        screens->addBootStatusLine("HostName: " + config->getHostName());
    }
    else
    {
        log_println("Could not set HostName: " + config->getHostName());
        screens->addBootStatusLine("Could not set HostName: " + config->getHostName());
    }

    if (config->getStaSsid() != "" && config->getStaPassword() != "")
    {
        log_println("Connecting to STA " + config->getStaSsid() + "...");
        screens->addBootStatusLine("Connecting to STA " + config->getStaSsid() + "...");
        WiFi.begin(config->getStaSsid().c_str(), config->getStaPassword().c_str());
        wl_status_t status = (wl_status_t)WiFi.waitForConnectResult(5000);
        if (WL_CONNECTED == status)
        {
            log_println("STA " + config->getStaSsid() + ", IP " + WiFi.localIP().toString());
            screens->addBootStatusLine("STA " + config->getStaSsid() + ", IP " + WiFi.localIP().toString());
        }
        else
        {
            log_println("Could not connect STA " + config->getStaSsid() + "!");
            screens->addBootStatusLine("Could not connect STA " + config->getStaSsid() + "!");
        }
    }
    else
    {
        log_println("STA not configured, skipping.");
        screens->addBootStatusLine("STA not configured, skipping.");
    }

    log_println("Creating AP " + config->getApSsid() + "...");
    screens->addBootStatusLine("Creating AP " + config->getApSsid() + "...");
    if (WiFi.softAP(config->getApSsid().c_str(), config->getApPassword().c_str()))
    {
        log_println("AP " + config->getApSsid() + ", PW " + config->getApPassword() + ", IP " + WiFi.softAPIP().toString());
        screens->addBootStatusLine("AP " + config->getApSsid() + ", PW " + config->getApPassword() + ", IP " + WiFi.softAPIP().toString());
    }
    else
    {
        log_println("Could not start AP " + config->getApSsid() + "!");
        screens->addBootStatusLine("Could not start AP " + config->getApSsid() + "!");
    }

    if (WiFi.isConnected())
        screens->addBootStatusLine("STA " + config->getStaSsid() + ", IP " + WiFi.localIP().toString());
    else
        screens->addBootStatusLine("STA: not connected!");

    if (MDNS.begin(config->getHostName().c_str()))
    {
        MDNS.addService("http", "tcp", 80);
        log_println("MDNS: " + config->getHostName() + ".local");
        screens->addBootStatusLine("MDNS: " + config->getHostName() + ".local");
    }
    else
    {
        log_println("Could not start MDNS: " + config->getHostName() + ".local");
        screens->addBootStatusLine("MDNS: could not enable!");
    }

    timer->every(3000, [](void *)
                 {if (!WiFi.isConnected())WiFi.reconnect(); return true; });
}

void VevorWifi::onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
    switch (event)
    {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        tft->setStaStatus(CONNECTED);
        break;
    case ARDUINO_EVENT_WIFI_STA_STOP:
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        tft->setStaStatus(DISCONNECTED);
        break;
    case ARDUINO_EVENT_WIFI_STA_START:
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        tft->setStaStatus(UNKNOWN);
        break;
    case ARDUINO_EVENT_WIFI_AP_START:
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
        tft->setApStatus(CONNECTED, WiFi.softAPgetStationNum());
        break;
    case ARDUINO_EVENT_WIFI_AP_STOP:
        tft->setApStatus(DISCONNECTED, WiFi.softAPgetStationNum());
        break;

    default:
        break;
    }
}