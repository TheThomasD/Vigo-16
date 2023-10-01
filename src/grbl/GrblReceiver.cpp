#include "GrblReceiver.h"
#include "../log/Logger.h"

String GrblReceiver::grblVersion = "";

String GrblReceiver::getGrblVersion()
{
    return grblVersion;
}

void GrblReceiver::processReceivedData()
{
    if (serial->available())
    {
        if (!connected)
        {
            expectedReplyAt = -1;
            onConnected();
        }

        while (serial->available())
            readData();
    }
}

void GrblReceiver::readData()
{
    bool foundNewLine = false;
    while (serial->available() && !foundNewLine)
    {
        readBuffer[currentIndex] = serial->read();
        if (readBuffer[currentIndex] == '\n')
            foundNewLine = true;
        else
            currentIndex++;
        if (currentIndex > MAX_READ_BUFFER)
        {
            Serial.println("Overflow in GRBL serial read!");
            delay(5000);
            throw std::invalid_argument("Buffer size exceeded!");
        }
    }
    if (foundNewLine)
    {
        readBuffer[currentIndex] = '\0'; // replace \n and null terminate (for String processing)
        processLine();
        currentIndex = 0;
    }
}

void GrblReceiver::processLine()
{
    const String line = String(readBuffer);
    log_println("Received: " + line);

    switch (readBuffer[0])
    {
    case '<':
        log_println("Status");
        processStatusLine(line);
        break;
    case 'A':
        log_println("Alarm");
        break;
    case 'G':
        processGrblLine(line);
        break;
    case '$':
        log_println("Config");
        break;
    case '[':
        log_println("Message");
        break;
    case 'o':
        log_println("OK");
        break;
    }
}

void GrblReceiver::processGrblLine(const String line)
{
    const u_int8_t spaceIndex = line.indexOf(' ', 5); // next space after "Grbl "
    if (spaceIndex > 0)
        grblVersion = line.substring(0, spaceIndex);
}

void GrblReceiver::processStatusLine(const String line)
{
    const GrblStatusParser::GrblStatus status = statusParser.parse(line);
    if (statusCallback != nullptr)
        statusCallback(status);
}

void GrblReceiver::expectReply(long futureMillis)
{
    if (expectedReplyAt == -1)
    {
        expectedReplyAt = futureMillis;
    }
    else if (expectedReplyAt < millis())
    {
        expectedReplyAt = futureMillis;
        onDisconnected();
    }
}

void GrblReceiver::onStatusReceived(OnStatusReceivedCb callback)
{
    statusCallback = callback;
}

void GrblReceiver::onConnected()
{
    connected = true;
    tft->setSerialStatus(VevorST7735::Connected);
}

void GrblReceiver::onDisconnected()
{
    connected = false;
    tft->setSerialStatus(VevorST7735::Disconnected);
}

String GrblReceiver::toString(GrblStatusParser::GrblState state)
{
#define COMMAND_GRBL_STATE_TOSTRING(STATE) \
    if (GrblStatusParser::STATE == state)  \
        return #STATE;
    FOREACH_GRBL_STATE(COMMAND_GRBL_STATE_TOSTRING)
    return "Unknown";
}

GrblStatusParser::GrblState GrblReceiver::fromString(String state)
{
#define COMMAND_GRBL_STATE_FROMSTRING(STATE) \
    if (state == #STATE)                     \
        return GrblStatusParser::STATE;
    FOREACH_GRBL_STATE(COMMAND_GRBL_STATE_FROMSTRING)
    return GrblStatusParser::Unknown;
}