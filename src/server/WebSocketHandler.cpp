#include "WebSocketHandler.h"
#include "../log/Logger.h"

void WebSocketHandler::init(GrblSender *grblSender)
{
    this->grblSender = grblSender;
}

void WebSocketHandler::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        log_println("WS client connected...");
        client->text("Connected!");
        break;
    case WS_EVT_DISCONNECT:
        log_println("WS client disconnected...");
        break;
    case WS_EVT_ERROR:
        log_println("WS client error...");
        break;
    case WS_EVT_DATA:
        log_print("WS received data: ");
        data[len] = 0;
        log_println((char *) data);
        client->text((char *) data);
        break;
    case WS_EVT_PONG:
        log_println("WS received pong...");
        break;

    default:
        log_println("Received unknown WS event type!");
        break;
    }
}