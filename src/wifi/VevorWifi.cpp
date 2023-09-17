#include "VevorWifi.h"
#include <WiFi.h>
#include "../logging/Logger.h"

void VevorWifi::startWifi(VevorConfig *config)
{
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.disconnect();
    WiFi.softAPdisconnect();
    if (config->getStaSsid() != "" && config->getStaPassword() != "") {
        log_println("Connecting STA...");
        WiFi.begin(config->getStaSsid().c_str(), config->getStaPassword().c_str());
        wl_status_t status = (wl_status_t) WiFi.waitForConnectResult(5000);
        if (WL_CONNECTED == status) {
            log_println("STA connected!");
        } else {
            log_println("Could not connect STA!");
        }
    } else {
        log_println("STA not configured!");
    }
    
    if (WiFi.softAP(config->getApSsid().c_str(), config->getApPassword().c_str())) {
        log_println("AP ready!");
    } else {
        log_println("Could not start AP!");
    }

}