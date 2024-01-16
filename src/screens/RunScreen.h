#pragma once
#include "AScreen.h"
#include <SD.h>

class RunScreen : public AScreen
{
public:
    RunScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, VevorConfig *config, SwitchScreenCb switchScreenCb) : AScreen(tft, timer, buttons, config, switchScreenCb){
        running = false;
    };
    void showHook();
    void hideHook();
    void setFile(File file);
protected:
    void showProgress();
    void run();
    boolean processNextBytes();

private:
    File fileToRun;
    size_t processedBytes;
    bool running;
};