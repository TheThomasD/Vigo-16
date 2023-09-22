#pragma once

#include <Adafruit_ST7735.h>
#include "VevorSPI.h"

enum Status {UNKNOWN, DISCONNECTED, CONNECTED };

class VevorST7735 : public Adafruit_ST7735
{

public:
    VevorST7735();
    void init();
    void addBootStatusLine(String line);
    void setStaStatus(Status status);
    void setApStatus(Status status, uint8_t numberOfClients);
    void setSerialStatus(Status status);

    void demo();

protected:
  void redrawStatus();
  uint16_t getColor(Status status);
  static VevorSPI tftSpi;
  String status[5];
  Status staStatus = UNKNOWN;
  Status apStatus = UNKNOWN;
  uint8_t apClients = 0;
  Status serialStatus = UNKNOWN;
};