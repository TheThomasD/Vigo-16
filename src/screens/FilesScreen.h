#pragma once
#include "AScreen.h"
#include "../sd/SdCard.h"
#include <SD.h>

class FilesScreen : public AScreen
{
public:
    FilesScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, VevorConfig *config, SwitchScreenCb switchScreenCb) : AScreen(tft, timer, buttons, config, switchScreenCb)
    {
        this->sdCard = new SdCard();
        currentPath = File();
        currentFile = File();
        currentPathPosition = 0;
        wasDisconnected = false;
    };
    void showHook();
    void hideHook();
protected:
    void showFiles();

private:
    SdCard *sdCard;
    File currentPath;
    long currentPathPosition;
    File currentFile;
    bool wasDisconnected;
};