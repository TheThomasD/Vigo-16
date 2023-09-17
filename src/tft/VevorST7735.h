#include <Adafruit_ST7735.h>
#include "VevorSPI.h"

#define TFT_CS 15
#define TFT_RST 2
#define TFT_DC 12

class VevorST7735 : public Adafruit_ST7735
{

public:
    VevorST7735();
    void init();
    void demo();

protected:
  static VevorSPI tftSpi;
};