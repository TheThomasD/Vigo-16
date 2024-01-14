#pragma once

class SdCard
{
public:
    void mount();
    void unmount();
    uint64_t getTotalSpaceMB();
    uint64_t getUsedSpaceMB();
private:
    bool mounted = false;
};