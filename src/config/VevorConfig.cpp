#include "VevorConfig.h"

void VevorConfig::load()
{
    prefs.begin(storageNamespace);
    apSsid = prefs.getString(nameApSsid, "Vevor-CNC");
    apPassword = prefs.getString(nameApPassword, "12345678");
    staSsid = prefs.getString(nameStaSsid, "");
    staPassword = prefs.getString(nameStaPassword, "");
    hostName = prefs.getString(nameHostName, "Vevor-CNC");
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
    prefs.end();
}

void VevorConfig::printPair(const char *name, String value)
{
    Serial.print(name);
    Serial.print(": ");
    Serial.println(value);
}

void VevorConfig::print()
{
    printPair(nameApSsid, apSsid);
    printPair(nameApPassword, apPassword);
    printPair(nameStaSsid, staSsid);
    printPair(nameStaPassword, staPassword);
    printPair(nameHostName, hostName);
}

void VevorConfig::setApSsid(String ssid)
{
    apSsid = ssid;
}

String VevorConfig::getApSsid()
{
    return apSsid;
}

void VevorConfig::setApPassword(String password)
{
    apPassword = password;
}

String VevorConfig::getApPassword()
{
    return apPassword;
}

void VevorConfig::setStaSsid(String ssid)
{
    staSsid = ssid;
}

String VevorConfig::getStaSsid()
{
    return staSsid;
}

void VevorConfig::setStaPassword(String password)
{
    staPassword = password;
}

String VevorConfig::getStaPassword()
{
    return staPassword;
}

void VevorConfig::setHostName(String name)
{
    hostName = name;
}

String VevorConfig::getHostName()
{
    return hostName;
}