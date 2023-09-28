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