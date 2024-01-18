#pragma once
#include "AScreen.h"
#include <SD.h>
#include "../grbl/GrblSender.h"

class RunScreen : public AScreen
{
public:
    RunScreen(VevorST7735 *tft, Timer<> *timer, VevorButtons *buttons, VevorConfig *config, SwitchScreenCb switchScreenCb, GrblSender* sender) : AScreen(tft, timer, buttons, config, switchScreenCb){
        running = false;
        this->sender = sender;
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
    GrblSender *sender;
};