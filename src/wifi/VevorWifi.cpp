#include "VevorWifi.h"
#include "../log/Logger.h"
#include <ESPmDNS.h>

VevorWifi::VevorWifi(VevorST7735 *tft)
{
    this->tft = tft;
}

void VevorWifi::startWifi(VevorConfig *config, Timer<> *timer)
{
    tft->addBootStatusLine("Connecting Wifi...");
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.disconnect();
    WiFi.softAPdisconnect();

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
                 { this->onWiFiEvent(event, info); },
                 ARDUINO_EVENT_MAX);

    if (WiFi.setHostname(config->getHostName().c_str()))
    {
        log_println("HostName: " + config->getHostName());
        tft->addBootStatusLine("HostName: " + config->getHostName());
    }
    else
    {
        log_println("Could not set HostName: " + config->getHostName());
        tft->addBootStatusLine("Could not set HostName: " + config->getHostName());
    }

    if (config->getStaSsid() != "" && config->getStaPassword() != "")
    {
        log_println("Connecting to STA " + config->getStaSsid() + "...");
        tft->addBootStatusLine("Connecting to STA " + config->getStaSsid() + "...");
        WiFi.begin(config->getStaSsid().c_str(), config->getStaPassword().c_str());
        wl_status_t status = (wl_status_t)WiFi.waitForConnectResult(5000);
        if (WL_CONNECTED == status)
        {
            log_println("STA " + config->getStaSsid() + ", IP " + WiFi.localIP().toString());
            tft->addBootStatusLine("STA " + config->getStaSsid() + ", IP " + WiFi.localIP().toString());
        }
        else
        {
            log_println("Could not connect STA " + config->getStaSsid() + "!");
            tft->addBootStatusLine("Could not connect STA " + config->getStaSsid() + "!");
        }
    }
    else
    {
        log_println("STA not configured, skipping.");
        tft->addBootStatusLine("STA not configured, skipping.");
    }

    log_println("Creating AP " + config->getApSsid() + "...");
    tft->addBootStatusLine("Creating AP " + config->getApSsid() + "...");
    if (WiFi.softAP(config->getApSsid().c_str(), config->getApPassword().c_str()))
    {
        log_println("AP " + config->getApSsid() + ", PW " + config->getApPassword() + ", IP " + WiFi.softAPIP().toString());
        tft->addBootStatusLine("AP " + config->getApSsid() + ", PW " + config->getApPassword() + ", IP " + WiFi.softAPIP().toString());
    }
    else
    {
        log_println("Could not start AP " + config->getApSsid() + "!");
        tft->addBootStatusLine("Could not start AP " + config->getApSsid() + "!");
    }

    if (WiFi.isConnected())
        tft->addBootStatusLine("STA " + config->getStaSsid() + ", IP " + WiFi.localIP().toString());
    else
        tft->addBootStatusLine("STA: not connected!");

    if (MDNS.begin(config->getHostName().c_str()))
    {
        log_println("MDNS: " + config->getHostName() + ".local");
        tft->addBootStatusLine("MDNS: " + config->getHostName() + ".local");
    }
    else
    {
        log_println("Could not start MDNS: " + config->getHostName() + ".local");
        tft->addBootStatusLine("MDNS: could not enable!");
    }

    timer->every(5000, [](void *)
                 {
                     if (!WiFi.isConnected())
                         WiFi.reconnect();
                     return true;
                 });
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