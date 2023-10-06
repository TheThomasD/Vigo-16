#include "VevorServer.h"
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include "../log/Logger.h"

void VevorServer::init(AsyncWebServer *server)
{
    if (SPIFFS.begin(true))
        log_println("SPIFFS mounted.");
    else
        log_println("SPIFFS could not be mounted!");

    server->onNotFound([](AsyncWebServerRequest *request)
                       { request->send(404); });

    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->redirect("/index.html"); });

    server->serveStatic("/", SPIFFS, "/");

    AsyncElegantOTA.begin(server);

    server->begin();
}