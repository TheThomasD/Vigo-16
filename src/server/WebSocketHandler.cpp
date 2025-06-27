#include "WebSocketHandler.h"
#include "../log/Logger.h"
#include "../sd/SdCard.h"
#include <SD.h>
#include <ArduinoJson.h>
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

        if (data[0] == '{' && data[len - 1] == '}')
        {
            log_println("JSON data received!");
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, data);
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                return;
            }

            String type = doc["type"].as<String>();
            if (type == "files")
            {

                File currentPath = File();
                File currentFile = File();
                long currentPathPosition = 0;
                SdCard *sdCard = new SdCard();
                sdCard->mount();
                if (!sdCard->isMounted())
                {
                    log_println("SD card not mounted!");
                    return;
                }
                bool canShow = sdCard->ensureMounted();
                if (canShow)
                {
                    currentPath = sdCard->getRoot();
                    currentPath.rewindDirectory();

                    for (int i = 0; i < currentPathPosition; i++)
                        currentPath.openNextFile().close();
                    currentFile.close();
                    currentFile = File();

                    JsonDocument responseDoc;
                    responseDoc["type"] = "files";
                    JsonArray filesArray = responseDoc.createNestedArray("files");
                    for (int i = 0; true; i++)
                    {
                        File file = currentPath.openNextFile();
                        if (file)
                        {
                            JsonObject fileObj = filesArray.createNestedObject();
                            fileObj["name"] = file.name();
                            fileObj["path"] = file.path();

                            if (!currentFile)
                                currentFile = file;
                            else
                                file.close();
                        }
                        else
                            break;
                    }

                    String jsonResponse;
                    serializeJson(responseDoc, jsonResponse);
                    client->text(jsonResponse);

                    sdCard->unmount();
                }
                return;
            }
        }
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