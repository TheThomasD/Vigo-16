#include "GrblController.h"
#include "../log/Logger.h"

void GrblController::init(VevorWifi *wifi)
{
    tft->setSerialStatus(VevorST7735::Unknown);

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
        3000, [](void *receiver)
        {
            ((GrblReceiver *) receiver)->checkConnection();
            return true; },
        this->receiver);

    timer->every(
        500, [](void *receiver)
        {
            ((GrblReceiver *) receiver)->checkForStatus();
            return true; },
        this->receiver);

    wifi->onClientMessage([this](const char *buffer, const size_t size)
                          { serial->write(buffer, size); });

    receiver->onMessageReceived([wifi](const char *buffer, const size_t size)
                                { wifi->sendToClient(buffer, size); });
}