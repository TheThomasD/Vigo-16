#include "GrblReceiver.h"
#include "../log/Logger.h"

// #define DEBUG

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
    messageLine = String(readBuffer);
#ifdef DEBUG
    log_println("Received: " + messageLine);
#endif

    if (messageCallback)
        messageCallback(messageLine);

    switch (readBuffer[0])
    {
    case '<':
#ifdef DEBUG
        log_println("Status");
#endif
        processStatusLine(messageLine);
        break;
    case 'A':
#ifdef DEBUG
        log_println("Alarm");
#endif
        break;
    case 'G':
        processGrblLine(messageLine);
        break;
    case '$':
#ifdef DEBUG
        log_println("Config");
#endif
        break;
    case '[':
#ifdef DEBUG
        log_println("Message");
#endif
        break;
    case 'o':
#ifdef DEBUG
        log_println("OK");
#endif
        break;
    }
}

void GrblReceiver::onMessageReceived(OnMessageReceivedCb callback)
{
    messageCallback = callback;
}

void GrblReceiver::processGrblLine(const String &line)
{
    const u_int8_t spaceIndex = line.indexOf(' ', 5); // next space after "Grbl "
    if (spaceIndex > 0)
        grblVersion = line.substring(0, spaceIndex);
}

void GrblReceiver::processStatusLine(const String &line)
{
    if (statusCallback != nullptr)
        statusCallback(*statusParser.parse(line));
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

GrblStatusParser::GrblState GrblReceiver::fromString(const String *state)
{
#define COMMAND_GRBL_STATE_FROMSTRING(STATE) \
    if (*state == #STATE)                    \
        return GrblStatusParser::STATE;
    FOREACH_GRBL_STATE(COMMAND_GRBL_STATE_FROMSTRING)
    return GrblStatusParser::Unknown;
}