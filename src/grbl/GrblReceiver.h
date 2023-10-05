#pragma once

#include <WString.h>
#include "../tft/VevorST7735.h"
#include "parser/GrblStatusParser.h"
#include "GrblSender.h"

class GrblReceiver
{
public:
    typedef std::function<void(const String&)> OnMessageReceivedCb;
    typedef std::function<void(const GrblStatusParser::GrblStatus&)> OnStatusReceivedCb;
    enum GrblAlarm
    {
        HardLimit = 1,
        MachineTravelExceeded = 2,
        ResetWhileInMotion = 3,
        ProbeInitFail = 4,
        ProbingFailed = 5,
        ResetDuringHoming = 6,
        HomingDoorOpen = 7,
        HomingLimitSwitchNotCleared = 8,
        HomingFailed = 9
    };
    typedef std::function<void(GrblAlarm)> OnAlarmReceivedCb;

    GrblReceiver(HardwareSerial *serial, GrblSender *sender, VevorST7735 *tft)
    {
        this->serial = serial;
        this->sender = sender;
        this->tft = tft;
    };
    void processReceivedData();
    void checkConnection();
    void checkForStatus();

    void onMessageReceived(OnMessageReceivedCb callback);
    void onStatusReceived(OnStatusReceivedCb callback);
    void onAlarmReceived(OnAlarmReceivedCb callback);

    static String getGrblVersion();
    String toString(GrblStatusParser::GrblState state);

    bool isConnected() { return connected; };

protected:
    void readData();
    void processLine();

    void processVersionLine();
    void processAlarmLine();
    void processStatusLine(const String& line);
    void processSettingsLine();
    void processGrblLine(const String& line);

    void onDisconnected();
    void onConnected();
    GrblStatusParser::GrblState fromString(const String *state);

private:
    HardwareSerial *serial;
    VevorST7735 *tft;
    GrblSender *sender;

    GrblStatusParser statusParser;

    bool connected = false;
    u_long lastStatusAt = -1;
    u_long lastMessageAt = -1;
    OnStatusReceivedCb statusCallback = nullptr;
    OnAlarmReceivedCb alarmCallback = nullptr;
    OnMessageReceivedCb messageCallback = nullptr;

    static String grblVersion;

    u_int8_t currentIndex = 0;
#define MAX_READ_BUFFER 128
    char readBuffer[MAX_READ_BUFFER + 1];
    String messageLine = "";
};