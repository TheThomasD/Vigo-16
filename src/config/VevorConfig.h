#pragma once

#include <WString.h>
#include <Preferences.h>

class VevorConfig
{
public:
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

protected:
    Preferences prefs;

    void printPair(const char* name, String value);

private:
    String apSsid;
    String apPassword;
    String staSsid;
    String staPassword;

    const char *storageNamespace = "vigo-16";
    const char *nameApSsid = "apSsid";
    const char *nameApPassword = "apPw";
    const char *nameStaSsid = "staSsid";
    const char *nameStaPassword = "staPw";
};