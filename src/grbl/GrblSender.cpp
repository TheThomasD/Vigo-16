#include "GrblSender.h"
#include "../log/Logger.h"
#include <WString.h>

// #define DEBUG

void GrblSender::queryStatus()
{
    if (serial->availableForWrite())
    {
        serial->print("?");
#ifdef DEBUG
        log_println("Sent!");
#endif
    }
}

void GrblSender::sendHome()
{
    serial->println("$H");
}

void GrblSender::sendUnlock()
{
    serial->println("$X");
}

void GrblSender::sendReset()
{
    serial->println(char(24)); // = ctrl-x
}

void GrblSender::sendProbe()
{
    sendSpindelStop();
    serial->println("G21G91\nG38.2Z-41F100\nG0Z1\nG38.2Z-2F10\nG1Z1.2F50");
}

void GrblSender::sendSpindleSpeed(uint8_t percent)
{
    String motorOnCommand = String("S") + String(percent * 100) + "M3"; // Max: 10.000
#ifdef DEBUG
    log_println("sending " + motorOnCommand);
#endif
    serial->println(motorOnCommand);
}

void GrblSender::sendSpindelStop()
{
    serial->println("M5");
}

void GrblSender::sendJog(Axis axis, float units, uint16_t feedRate)
{
    String jogCommand = String("$J=G91") + String(getAxisChar(axis)) + String(units) + 'F' + String(feedRate); // relative move
#ifdef DEBUG
    log_println("Sending " + jogCommand);
#endif
    serial->println(jogCommand);
}

char GrblSender::getAxisChar(Axis axis)
{
    switch (axis)
    {
    case X:
        return 'X';

    case Y:
        return 'Y';

    case Z:
    default:
        return 'Z';
    }
}