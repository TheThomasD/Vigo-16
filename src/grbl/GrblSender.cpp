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
    serial->println("G21G91\nG38.2Z-41F100\nG0Z1\nG38.2Z-2F10\nG92Z1.5\nG1Z1.5F50");
}

void GrblSender::sendSpindleSpeed(uint8_t percent)
{
#ifdef DEBUG
    log_print("Sending S");
    log_print(percent * 100);
    log_println("M3");
#endif
    // Avoid String concatenation in hot path - write directly to serial
    serial->print("S");
    serial->print(percent * 100);
    serial->println("M3");
}

void GrblSender::sendSpindelStop()
{
    serial->println("M5");
}

void GrblSender::sendJog(Axis axis, float units, uint16_t feedRate)
{
#ifdef DEBUG
    log_print("Sending $J=G91");
    log_print(getAxisChar(axis));
    log_print(units);
    log_print('F');
    log_println(feedRate);
#endif
    // Avoid String concatenation in hot path - write directly to serial
    serial->print("$J=G91");
    serial->print(getAxisChar(axis));
    serial->print(units);
    serial->print('F');
    serial->println(feedRate);
}

void GrblSender::sendGcode(char *code)
{
    serial->print(code);
}

void GrblSender::sendByte(const int byte)
{
    while (serial->availableForWrite() == 0)
        delay(1);
    serial->write(byte);
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
