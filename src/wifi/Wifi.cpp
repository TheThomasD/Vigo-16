#include <WiFi.h>

void testWifi()
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.printDiag(Serial);
    WiFi.softAP("Vevor", "12345678");
    Serial.println(WiFi.softAPIP());
    WiFi.printDiag(Serial);
}