#pragma once

#include <Adafruit_ST7735.h>
#include "VevorSPI.h"

class VevorST7735 : public Adafruit_ST7735
{

public:
    VevorST7735();
    void init();
    void addStatusLine(String line);

    void demo();

protected:
  static VevorSPI tftSpi;
  String status[5];
};