#include "VevorServer.h"
#include <SPIFFS.h>
#include <ElegantOTA.h>
#include "../log/Logger.h"

void VevorServer::init(AsyncWebServer *server, AsyncWebSocket *webSocket, WebSocketHandler *handler)
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

    webSocket->onEvent([handler](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
                       { handler->onEvent(server, client, type, arg, data, len); });
    server->addHandler(webSocket);

    ElegantOTA.begin(server);

    server->begin();
}