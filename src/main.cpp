#include "Arduino.h"

#include <tft/VevorST7735.h>
#include <audio/VevorSpeaker.h>
#include <config/VevorConfig.h>
#include <wifi/VevorWifi.h>
#include <ESPAsyncWebServer.h>
#include "log/Logger.h"
#include <server/VevorServer.h>

VevorST7735 tft = VevorST7735();
VevorConfig config = VevorConfig();
VevorWifi wifi = VevorWifi();

AsyncWebServer webServer(80);
VevorServer server;

void setup(void)
{
  Serial.begin(115200);
  Serial1.begin(115200);

  VevorSpeaker::playTone(NOTE_C5, 100);
  VevorSpeaker::playTone(NOTE_E5, 100);
  VevorSpeaker::playTone(NOTE_G5, 100);

  delay(1000);

  config.load();
  config.print();

  tft.init();

  wifi.startWifi(&config, &tft);

  server.init(&webServer);

  Serial.println(F("Initialized"));
}

void loop()
{
  delay(10);
}