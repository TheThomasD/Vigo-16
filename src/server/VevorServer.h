#pragma once
#include <ESPAsyncWebServer.h>
#include "WebSocketHandler.h"

class VevorServer {
    public:
        void init(AsyncWebServer *server, AsyncWebSocket *webSocket, WebSocketHandler *handler);
};