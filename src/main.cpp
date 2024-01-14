#include <Arduino.h>
#include <tft/VevorST7735.h>
#include <audio/VevorSpeaker.h>
#include <config/VevorConfig.h>
#include <wifi/VevorWifi.h>
#include <ESPAsyncWebServer.h>
#include "log/Logger.h"
#include <server/VevorServer.h>
#include <server/WebSocketHandler.h>
#include <arduino-timer.h>
#include <screens/VevorScreens.h>
#include <buttons/VevorButtons.h>
#include <grbl/GrblController.h>

AsyncWebServer webServer(80);
AsyncWebSocket webSocket("/ws");

VevorSpeaker speaker = VevorSpeaker();
VevorST7735 tft = VevorST7735();
VevorConfig config = VevorConfig();
VevorWifi wifi = VevorWifi(&tft);
Timer<> timer = timer_create_default();
GrblController grbl = GrblController(&Serial1, &config, &tft, &timer, &webSocket);
VevorButtons buttons = VevorButtons(&timer);
VevorScreens screens = VevorScreens(&tft, &timer, &buttons, &config, grbl.getSender(), grbl.getReceiver());


VevorServer server;
WebSocketHandler webSocketHandler;

void setup(void)
{
  Serial.begin(115200);
  log_println("Start...");

  speaker.playTone(NOTE_C5, 100);
  speaker.playTone(NOTE_E5, 100);
  speaker.playTone(NOTE_G5, 100);

  log_println("Loading config...");
  config.load();

  log_println("Init display...");
  tft.init();

  log_println("Init buttons...");
  buttons.init();

  log_println("Loading boot screen...");
  screens.showBootScreen();

  log_println("Starting WiFi...");
  wifi.startWifi(&config, &timer, &screens);

  log_println("Init GRBL serial...");
  grbl.init(&wifi);

  webSocketHandler.init(grbl.getSender());

  log_println("Starting web server...");
  server.init(&webServer, &webSocket, &webSocketHandler);

  log_println("Initialization done!");

  timer.in(
      3000, [](void *screens)
      {
    ((VevorScreens *) screens)->showMenuScreen();
    return false; },
      &screens);
}

void loop()
{
  timer.tick();
}