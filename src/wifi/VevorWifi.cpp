#include "VevorWifi.h"
#include <WiFi.h>
#include "../logging/Logger.h"

void VevorWifi::startWifi(VevorConfig *config, VevorST7735* tft)
{
    tft->addStatusLine("Connecting Wifi...");
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.disconnect();
    WiFi.softAPdisconnect();
    if (config->getStaSsid() != "" && config->getStaPassword() != "") {
        log_println("Connecting STA...");
        tft->addStatusLine("Connecting to STA " + config->getStaSsid() + "...");
        WiFi.begin(config->getStaSsid().c_str(), config->getStaPassword().c_str());
        wl_status_t status = (wl_status_t) WiFi.waitForConnectResult(5000);
        if (WL_CONNECTED == status) {
            log_println("STA connected!");
            tft->addStatusLine("STA " + config->getStaSsid() + ", IP " + WiFi.localIP().toString() + "!");
        } else {
            log_println("Could not connect STA!");
            tft->addStatusLine("Could not connect STA " + config->getStaSsid() + "!");
        }
    } else {
        log_println("STA not configured!");
        tft->addStatusLine("STA not configured, skipping.");
    }
    
    tft->addStatusLine("Creating AP " + config->getApSsid() + "...");
    if (WiFi.softAP(config->getApSsid().c_str(), config->getApPassword().c_str())) {
        log_println("AP ready!");
        tft->addStatusLine("AP " + config->getApSsid() + ", PW " + config->getApPassword() + ", IP " + WiFi.softAPIP().toString());
    } else {
        log_println("Could not start AP!");
        tft->addStatusLine("Could not start AP " + config->getApSsid() + "!");
    }

    if (WiFi.isConnected()) {
        tft->addStatusLine("STA " + config->getStaSsid() + ", IP " + WiFi.localIP().toString());
    } else {
        tft->addStatusLine("STA: not connected!");
    }   
}