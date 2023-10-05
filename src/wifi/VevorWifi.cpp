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
        wl_status_t status = WiFi.begin(config->getStaSsid().c_str(), config->getStaPassword().c_str());
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

    server = new WiFiServer(config->getTcpPort());
    server->begin();
    server->setNoDelay(true);

    timer->every(3000, [](void *)
                 {if (!WiFi.isConnected()) WiFi.reconnect(); return true; });

    timer->every(
        10, [](void *wifi)
        { 
            ((VevorWifi *) wifi)->handleWiFiServer();
            return true; },
        this);
}

void VevorWifi::handleWiFiServer()
{
    if ((!client || !client.connected()) && server->hasClient())
    {
        if (client)
            client.stop(); // disconnect existing client
        client = server->available();
    }
    if (clientMessageCb && client && client.connected() && client.available())
    {
        while (client.available())
        {
            size_t read = client.readBytes(messageBuffer, 128);
            messageBuffer[read] = '\0'; // add string terminator
            clientMessageCb(String(messageBuffer));
        }
    }
}

void VevorWifi::onClientMessage(const OnClientMessageCb callback)
{
    clientMessageCb = callback;
}

void VevorWifi::sendToClient(const String& message)
{
    if (client && client.connected())
        client.println(message);
}

void VevorWifi::onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
    switch (event)
    {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        tft->setStaStatus(VevorST7735::Connected);
        break;
    case ARDUINO_EVENT_WIFI_STA_STOP:
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        tft->setStaStatus(VevorST7735::Disconnected);
        break;
    case ARDUINO_EVENT_WIFI_STA_START:
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        tft->setStaStatus(VevorST7735::Unknown);
        break;
    case ARDUINO_EVENT_WIFI_AP_START:
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
        tft->setApStatus(VevorST7735::Connected, WiFi.softAPgetStationNum());
        break;
    case ARDUINO_EVENT_WIFI_AP_STOP:
        tft->setApStatus(VevorST7735::Disconnected, WiFi.softAPgetStationNum());
        break;

    default:
        break;
    }
}