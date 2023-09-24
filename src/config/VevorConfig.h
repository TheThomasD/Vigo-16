#pragma once

#include <WString.h>
#include <Preferences.h>

class VevorConfig
{
public:
#define FOREACH_BAUD_RATE(COMMAND) \
    COMMAND(9600)                  \
    COMMAND(19200)                 \
    COMMAND(28800)                 \
    COMMAND(38400)                 \
    COMMAND(57600)                 \
    COMMAND(76800)                 \
    COMMAND(115200)                \
    COMMAND(230400)                \
    COMMAND(460800)                \
    COMMAND(576000)                \
    COMMAND(921600)
#define COMMAND_BAUD_RATE_ENUM(RATE) BR_##RATE,
    enum BaudRate
    {
        FOREACH_BAUD_RATE(COMMAND_BAUD_RATE_ENUM)
    };

    void load();
    void save();
    void print();

    void setApSsid(String ssid);
    String getApSsid();
    void setApPassword(String password);
    String getApPassword();
    void setStaSsid(String ssid);
    String getStaSsid();
    void setStaPassword(String password);
    String getStaPassword();
    void setHostName(String hostname);
    String getHostName();
    void setBaudRate(BaudRate baudRate);
    BaudRate getBaudRate();
    uint32_t toValue(BaudRate rate);
    void setFeedRate(uint16_t feedRate);
    uint16_t getFeedRate();

protected:
    Preferences prefs;

    void printPair(const char *name, String value);
    BaudRate fromValue(uint32_t value);

private:
    String apSsid;
    String apPassword;
    String staSsid;
    String staPassword;
    String hostName;
    BaudRate baudRate;
    uint16_t feedRate;

    const char *storageNamespace = "vigo-16";
    const char *nameApSsid = "apSsid";
    const char *nameApPassword = "apPw";
    const char *nameStaSsid = "staSsid";
    const char *nameStaPassword = "staPw";
    const char *nameHostName = "host";
    const char *nameBaudRate = "bRate";
    const char *nameFeedRate = "fRate";
};