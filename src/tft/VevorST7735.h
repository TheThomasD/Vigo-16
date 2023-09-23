#pragma once

#include <Adafruit_ST7735.h>
#include "VevorSPI.h"

enum Status
{
  UNKNOWN,
  DISCONNECTED,
  CONNECTED
};

class VevorST7735 : public Adafruit_ST7735
{

public:
  VevorST7735();
  void init();

  void setStaStatus(Status status);
  void setApStatus(Status status, uint8_t numberOfClients);
  void setSerialStatus(Status status);
  void redrawStatus();
  void clear();

  //    void demo();

protected:
  static VevorSPI tftSpi;
  uint16_t getColor(Status status);

private:
  Status staStatus = UNKNOWN;
  Status apStatus = UNKNOWN;
  uint8_t apClients = 0;
  Status serialStatus = UNKNOWN;
};