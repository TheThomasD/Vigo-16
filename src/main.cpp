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
TaskHandle_t task_CPU0;
Timer<> timer_CPU0 = timer_create_default();
Timer<> timer_CPU1 = timer_create_default();
GrblController grbl = GrblController(&Serial1, &config, &tft, &timer_CPU1, &webSocket);
VevorButtons buttons = VevorButtons(&timer_CPU0);
VevorScreens screens = VevorScreens(&tft, &timer_CPU0, &buttons, &config, grbl.getSender(), grbl.getReceiver());
VevorServer server;
WebSocketHandler webSocketHandler;

void timerTask_CPU0(void * parameter);

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
  wifi.startWifi(&config, &timer_CPU1, &screens);

  log_println("Init GRBL serial...");
  grbl.init(&wifi);

  webSocketHandler.init(grbl.getSender());

  log_println("Starting web server...");
  server.init(&webServer, &webSocket, &webSocketHandler);

  log_println("Initialization done!");

  timer_CPU0.in(
      3000, [](void *screens)
      {
    ((VevorScreens *) screens)->showMenuScreen();
    return false; },
      &screens);

  xTaskCreatePinnedToCore(
      timerTask_CPU0, // Function to implement the task
      "task1",        // Name of the task
      4000,           // Stack size in words
      NULL,           // Task input parameter
      0,             // Priority of the task (lower = lower prio)
      &task_CPU0,           // Task handle. (can stop the task)
      0               // Core where the task should run
  );
  //vTaskDelete(task_CPU0); // delete the task created above
}

void timerTask_CPU0(void * parameter)
{
  while (true) {
    timer_CPU0.tick();
    delay(10);
  }
}

void loop()
{
  timer_CPU1.tick();
}