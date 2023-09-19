#pragma once

#include "../config/VevorConfig.h"
#include "../tft/VevorST7735.h"

class VevorWifi
{
public:
    void startWifi(VevorConfig* config, VevorST7735* tft);
};