#pragma once
#include <SD.h>

class SdCard
{
public:
    void mount();
    void unmount();
    uint64_t getTotalSpaceMB();
    uint64_t getUsedSpaceMB();
    File getRoot();
    uint16_t getNumberOfFiles(File path);
    bool isMounted();
    bool ensureMounted();
private:
    bool mounted = false;
};