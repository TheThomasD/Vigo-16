#include <Adafruit_ST7735.h>

#define TFT_CS        15
#define TFT_RST        2
#define TFT_DC        12

class Vevor_ST7735 : public Adafruit_ST7735 {

    public:
        Vevor_ST7735();
        void init();

};