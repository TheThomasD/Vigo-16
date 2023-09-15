#include "Vevor_ST7735.h"
#include "Vevor_SPI.h"
#include "../bmp/Vevor.h"

// TFT SPI (HSPI)
static Vevor_SPI tftSpi = Vevor_SPI();

Vevor_ST7735::Vevor_ST7735() : Adafruit_ST7735(&tftSpi, TFT_CS, TFT_DC, TFT_RST) {
};

void Vevor_ST7735::init() {
    initR(INITR_18BLACKTAB);
    setColRowStart(2,1);
    setRotation(1);
    fillScreen(ST7735_BLACK);
    drawRGBBitmap(0, 20, image_data_Vevor, 160, 60);
}