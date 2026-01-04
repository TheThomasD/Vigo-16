#include "VevorConfig.h"

void VevorConfig::load()
{
    prefs.begin(storageNamespace);
    apSsid = prefs.getString(nameApSsid, "Vevor-CNC");
    apPassword = prefs.getString(nameApPassword, "12345678");
    staSsid = prefs.getString(nameStaSsid, "");
    staPassword = prefs.getString(nameStaPassword, "");
    hostName = prefs.getString(nameHostName, "Vevor-CNC");
    baudRate = fromValue(prefs.getULong(nameBaudRate, 115200));
    feedRate = prefs.getUShort(nameFeedRate, 1000);
    tcpPort = prefs.getUShort(nameTcpPort, 8888);
    prefs.end();
}

void VevorConfig::save()
{
    prefs.begin(storageNamespace);
    prefs.putString(nameApSsid, apSsid);
    prefs.putString(nameApPassword, apPassword);
    prefs.putString(nameStaSsid, staSsid);
    prefs.putString(nameStaPassword, staPassword);
    prefs.putString(nameHostName, hostName);
    prefs.putULong(nameBaudRate, toValue(baudRate));
    prefs.putUShort(nameFeedRate, feedRate);
    prefs.putUShort(nameTcpPort, tcpPort);
    prefs.end();
}

void VevorConfig::printPair(const char *name, const String &value)
{
    Serial.print(name);
    Serial.print(F(": "));
    Serial.println(value);
}

void VevorConfig::print()
{
    printPair(nameApSsid, apSsid);
    printPair(nameApPassword, apPassword);
    printPair(nameStaSsid, staSsid);
    printPair(nameStaPassword, staPassword);
    printPair(nameHostName, hostName);
    printPair(nameBaudRate, String(toValue(baudRate)));
    printPair(nameFeedRate, String(feedRate));
    printPair(nameTcpPort, String(tcpPort));
}

uint32_t VevorConfig::toValue(BaudRate rate)
{
#define COMMAND_BAUD_RATE_TOVALUE(RATE) \
    if (rate == BR_##RATE)              \
        return RATE;
    FOREACH_BAUD_RATE(COMMAND_BAUD_RATE_TOVALUE)
    return 115200;
}

VevorConfig::BaudRate VevorConfig::fromValue(uint32_t value)
{
#define COMMAND_BAUD_RATE_FROMVALUE(RATE) \
    if (value == RATE)                    \
        return BR_##RATE;
    FOREACH_BAUD_RATE(COMMAND_BAUD_RATE_FROMVALUE)
    return BaudRate::BR_115200;
}
