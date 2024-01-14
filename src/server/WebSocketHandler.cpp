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
#ifdef DEBUG
        log_println("WS client connected...");
#endif
        client->text("Connection successful!");
        break;
    case WS_EVT_DISCONNECT:
#ifdef DEBUG
        log_println("WS client disconnected...");
#endif
        break;
    case WS_EVT_ERROR:
#ifdef DEBUG
        log_println("WS client error...");
#endif
        break;
    case WS_EVT_DATA:
#ifdef DEBUG
        log_print("WS received data: ");
#endif
        data[len] = 0;
        log_println((char *)data);
        grblSender->sendGcode((char *)data);
        break;
    case WS_EVT_PONG:
#ifdef DEBUG
        log_println("WS received pong...");
#endif
        break;

    default:
        log_println("Received unknown WS event type!");
        break;
    }
}