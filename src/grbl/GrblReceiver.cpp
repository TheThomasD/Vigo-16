#include "GrblReceiver.h"
#include "../log/Logger.h"

// #define DEBUG

char GrblReceiver::grblVersion[64] = "";

void GrblReceiver::processReceivedData()
{
    if (serial->available())
    {
        lastMessageAt = millis();
        if (!connected)
            onConnected();

        while (serial->available())
            readData();
    }
}

void GrblReceiver::readData()
{
    bool foundNewLine = false;
    while (serial->available() && !foundNewLine)
    {
        char c = serial->read();
        if (messageCallback)
            messageCallback(c);
        if (c == '\n')
        {
            foundNewLine = true;
        }
        else
        {
            readBuffer[currentIndex] = c;
            currentIndex++;
            if (currentIndex > MAX_READ_BUFFER)
            {
                Serial.println("Overflow in GRBL serial read!");
                delay(5000);
                throw std::invalid_argument("Buffer size exceeded!");
            }
        }
    }
    if (foundNewLine)
    {
        readBuffer[currentIndex] = '\0';  // replace \n and null terminate (for String processing)
        processLine();
        currentIndex = 0;
    }
}

void GrblReceiver::processLine()
{
    if (webSocket->count() > 0)
    {
        webSocket->textAll(readBuffer);
    }
#ifdef DEBUG
    log_print("Received: ");
    log_println(readBuffer);
#endif

    switch (readBuffer[0])
    {
    case '<':
#ifdef DEBUG
        log_println("Status");
#endif
        processStatusLine(readBuffer);  // pass C-string directly
        break;
    case 'A':
#ifdef DEBUG
        log_println("Alarm");
#endif
        break;
    case 'G':
        processGrblLine(readBuffer);    // pass C-string directly
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

void GrblReceiver::processGrblLine(const char* line)
{
    // Find space after "Grbl " (at position 5)
    const char* spacePtr = strchr(line + 5, ' ');
    if (spacePtr != nullptr)
    {
        size_t len = (size_t)(spacePtr - line);
        if (len >= sizeof(grblVersion))
            len = sizeof(grblVersion) - 1;
        memcpy(grblVersion, line, len);
        grblVersion[len] = '\0';
    }
}

void GrblReceiver::processStatusLine(const char* line)
{
    lastStatusAt = millis();
    if (statusCallback != nullptr)
        statusCallback(*statusParser.parse(line));
}

void GrblReceiver::checkConnection()
{
    if (lastMessageAt < millis() - 3000)
        onDisconnected();
}

void GrblReceiver::checkForStatus()
{
    if (lastStatusAt == -1 || lastStatusAt < millis() - 500)
        sender->queryStatus();
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