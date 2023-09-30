#include "GrblSender.h"
#include "../log/Logger.h"

#define QUERY_TIMEOUT 5000

void GrblSender::queryStatus()
{
    if (serial->availableForWrite())
    {
        serial->print("?");
        log_println("Sent!");

        receiver->expectReply(millis() + QUERY_TIMEOUT);
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

void GrblSender::sendSpindleSpeed(uint8_t percent)
{
    String motorOnCommand = String("S") + String(percent * 100) + "M3"; // Max: 10.000
    log_println("sending " + motorOnCommand);
    serial->println(motorOnCommand);
}

void GrblSender::sendSpindelStop()
{
    serial->println("M5");
}

void GrblSender::sendJog(Axis axis, float units, uint16_t feedRate)
{
    String jogCommand = String("$J=G91") + String(getAxisChar(axis)) + String(units) + 'F' + String(feedRate); // relative move
    log_println("Sending " + jogCommand);
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