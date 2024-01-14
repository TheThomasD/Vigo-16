#pragma once
#include <ESPAsyncWebServer.h>
#include "../grbl/GrblController.h"

class WebSocketHandler {
    public:
        void init(GrblSender *grblSender);
        void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
    private:
        GrblSender *grblSender;
};