#pragma once

#include <Adafruit_ST7735.h>
#include "VevorSPI.h"

class VevorST7735 : public Adafruit_ST7735
{
public:
  enum Status
  {
    Unknown,
    Disconnected,
    Connected
  };

  VevorST7735();
  void init();

  void setStaStatus(Status status);
  void setApStatus(Status status, uint8_t numberOfClients);
  void setSerialStatus(Status status);
  void redrawStatus();
  void clear();
  void setTitle(String title);

  //    void demo();

protected:
  static VevorSPI tftSpi;
  uint16_t getColor(Status status);

private:
  Status staStatus = Unknown;
  Status apStatus = Unknown;
  uint8_t apClients = 0;
  Status serialStatus = Unknown;
};