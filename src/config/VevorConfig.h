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

    // Inline getters and setters for better performance
    void setApSsid(const String &ssid) { apSsid = ssid; }
    const String &getApSsid() { return apSsid; }
    void setApPassword(const String &password) { apPassword = password; }
    const String &getApPassword() { return apPassword; }
    void setStaSsid(const String &ssid) { staSsid = ssid; }
    const String &getStaSsid() { return staSsid; }
    void setStaPassword(const String &password) { staPassword = password; }
    const String &getStaPassword() { return staPassword; }
    void setHostName(const String &hostname) { hostName = hostname; }
    const String &getHostName() { return hostName; }
    void setBaudRate(BaudRate baudRate) { this->baudRate = baudRate; }
    BaudRate getBaudRate() { return baudRate; }
    uint32_t toValue(BaudRate rate);
    void setFeedRate(uint16_t feedRate) { this->feedRate = feedRate; }
    uint16_t getFeedRate() { return feedRate; }
    void setTcpPort(uint16_t port) { tcpPort = port; }
    uint16_t getTcpPort() { return tcpPort; }

protected:
    Preferences prefs;

    void printPair(const char *name, const String &value);
    BaudRate fromValue(uint32_t value);

private:
    String apSsid;
    String apPassword;
    String staSsid;
    String staPassword;
    String hostName;
    BaudRate baudRate;
    uint16_t feedRate;
    uint16_t tcpPort;

    const char *storageNamespace = "vigo-16";
    const char *nameApSsid = "apSsid";
    const char *nameApPassword = "apPw";
    const char *nameStaSsid = "staSsid";
    const char *nameStaPassword = "staPw";
    const char *nameHostName = "host";
    const char *nameBaudRate = "bRate";
    const char *nameFeedRate = "fRate";
    const char *nameTcpPort = "port";
};
