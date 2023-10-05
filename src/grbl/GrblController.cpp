#include "GrblController.h"
#include "../log/Logger.h"

#define QUERY_DELAY 700

void GrblController::init(VevorWifi *wifi)
{
    tft->setSerialStatus(VevorST7735::Unknown);
    receiver->connected = false;

    serial->end();
    serial->begin(config->toValue(config->getBaudRate()));
    serial->setRxTimeout(2);

    serial->onReceive([this]()
                      { timer->in(
                            0, [](void *receiver)
                            {
                ((GrblReceiver *) receiver)->processReceivedData();
                return false; },
                            this->receiver); });

    timer->every(
        QUERY_DELAY, [](void *sender)
        {
            ((GrblSender *) sender)->queryStatus();
            return true; },
        this->sender);

    wifi->onClientMessage([this](String message)
                          { serial->print(message); });

    receiver->onMessageReceived([wifi](String message)
                                { wifi->sendToClient(message); });
}