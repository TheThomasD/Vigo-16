#include <Arduino.h>
#include <tft/VevorST7735.h>
#include <audio/VevorSpeaker.h>
#include <config/VevorConfig.h>
#include <wifi/VevorWifi.h>
#include <ESPAsyncWebServer.h>
#include "log/Logger.h"
#include <server/VevorServer.h>
#include <arduino-timer.h>
#include <screens/VevorScreens.h>
#include <buttons/VevorButtons.h>

VevorST7735 tft = VevorST7735();
VevorConfig config = VevorConfig();
VevorWifi wifi = VevorWifi(&tft);
Timer<> timer = timer_create_default();
VevorButtons buttons = VevorButtons(&timer);
VevorScreens screens = VevorScreens(&tft, &buttons);

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

  buttons.init();

  screens.showBootScreen();

  wifi.startWifi(&config, &timer, &screens);

  server.init(&webServer);

  log_println(F("Initialized"));

  delay(1000);

  screens.showMenuScreen();
}

void loop()
{
  timer.tick();
}