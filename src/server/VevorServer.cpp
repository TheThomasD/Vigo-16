#include "VevorServer.h"
#include <SPIFFS.h>
#include <ElegantOTA.h>
#include <SD.h>
#include "../sd/SdCard.h"
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

  server->on("/upload", HTTP_POST, [](AsyncWebServerRequest *request)
             { request->send(200, "text/plain", "File uploaded successfully"); }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
             {
               SdCard *sdCard = new SdCard();
               sdCard->mount();
               if (!sdCard->isMounted())
               {
                 log_println("SD card not mounted!");
                 return;
               }
               bool isMounted = sdCard->ensureMounted();
               if (isMounted)
               {

                 if (!index)
                 {
                   Serial.printf("UploadStart: %s\n", filename.c_str());
                   // Open the file for writing
                   request->_tempFile = SD.open("/" + filename, FILE_WRITE);
                 }
                 if (request->_tempFile)
                 {
                   request->_tempFile.write(data, len); // Write chunk
                 }
                 if (final)
                 {
                   if (request->_tempFile)
                   {
                     request->_tempFile.close();
                   }
                   Serial.printf("UploadEnd: %s (%u bytes)\n", filename.c_str(), index + len);
                 }
                 sdCard->unmount();
               } });

  server->on("/delete", HTTP_GET, [](AsyncWebServerRequest *request)
             {
               log_println("delete file");
               SdCard *sdCard = new SdCard();
               sdCard->mount();
               if (!sdCard->isMounted())
               {
                 log_println("SD card not mounted!");
                 request->send(500, "text/plain", "SD card not mounted!");

                 return;
               }
               bool isMounted = sdCard->ensureMounted();
               if (isMounted)
               {
                 String path = request->getParam("file")->value();
                 log_println("path: " + path);
                 if (!SD.exists(path))
                 {
                   request->send(404, "text/plain", "File not found");
                   return;
                 }
                 if (SD.remove(path))
                 {
                   Serial.println("Deleted: " + path);
                   request->send(200);
                 }
                 else
                 {
                   request->send(500, "text/plain", "Failed to delete file");
                 }
                 sdCard->unmount();
                 return;
               }
               request->send(500, "text/plain", "SD card not mounted!"); });

  server->on("/download", HTTP_GET, [](AsyncWebServerRequest *request)
             {
                log_println("download");

                if (!request->hasParam("file")) {
                  request->send(400, "text/plain", "Missing 'file' parameter");
                  return;
                }
                SdCard *sdCard = new SdCard();
                sdCard->mount();
                if (!sdCard->isMounted())
                {
                  log_println("SD card not mounted!");
                  return;
                }
                bool isMounted = sdCard->ensureMounted();
                if (isMounted)
                {
                  String path = request->getParam("file")->value();

                  if (!SD.exists(path)) {
                    request->send(404, "text/plain", "File not found");
                    return;
                  }
                  
                  log_println("Downloading file...");
                  request->send(SD, path, String(), true);
                  TimerHandle_t t = xTimerCreate("sdunmnt", pdMS_TO_TICKS(2000), pdFALSE, nullptr, [](TimerHandle_t) {
                    log_println("unmounting sd card");
                      SdCard sd;
                      sd.unmount();
                  });
                  xTimerStart(t, 0);
                  // log_println("unmounting sd card");
                  // sdCard->unmount();
              } });

  server->serveStatic("/", SPIFFS, "/");

  webSocket->onEvent([handler](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
                     { handler->onEvent(server, client, type, arg, data, len); });
  server->addHandler(webSocket);

  ElegantOTA.begin(server);

  server->begin();
}