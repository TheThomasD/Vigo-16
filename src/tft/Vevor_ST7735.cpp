#include "Vevor_ST7735.h"
#include "Vevor_SPI.h"
#include "../bmp/Vevor.h"

// TFT SPI (HSPI)
static Vevor_SPI tftSpi = Vevor_SPI();

Vevor_ST7735::Vevor_ST7735() : Adafruit_ST7735(&tftSpi, TFT_CS, TFT_DC, TFT_RST){};

void Vevor_ST7735::init()
{
    initR(INITR_18BLACKTAB);
    setColRowStart(2, 1);
    setRotation(1);
    fillScreen(ST7735_BLACK);
    drawRGBBitmap(0, 20, image_data_Vevor, 160, 60);
}

float p = 3.1415926;

void testlines(uint16_t color, Vevor_ST7735* tft)
{
    tft->fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < tft->width(); x += 6)
    {
        tft->drawLine(0, 0, x, tft->height() - 1, color);
        delay(0);
    }
    for (int16_t y = 0; y < tft->height(); y += 6)
    {
        tft->drawLine(0, 0, tft->width() - 1, y, color);
        delay(0);
    }

    tft->fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < tft->width(); x += 6)
    {
        tft->drawLine(tft->width() - 1, 0, x, tft->height() - 1, color);
        delay(0);
    }
    for (int16_t y = 0; y < tft->height(); y += 6)
    {
        tft->drawLine(tft->width() - 1, 0, 0, y, color);
        delay(0);
    }

    tft->fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < tft->width(); x += 6)
    {
        tft->drawLine(0, tft->height() - 1, x, 0, color);
        delay(0);
    }
    for (int16_t y = 0; y < tft->height(); y += 6)
    {
        tft->drawLine(0, tft->height() - 1, tft->width() - 1, y, color);
        delay(0);
    }

    tft->fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < tft->width(); x += 6)
    {
        tft->drawLine(tft->width() - 1, tft->height() - 1, x, 0, color);
        delay(0);
    }
    for (int16_t y = 0; y < tft->height(); y += 6)
    {
        tft->drawLine(tft->width() - 1, tft->height() - 1, 0, y, color);
        delay(0);
    }
}

void testdrawtext(char *text, uint16_t color, Vevor_ST7735* tft)
{
    tft->setCursor(0, 0);
    tft->setTextColor(color);
    tft->setTextWrap(true);
    tft->print(text);
}

void testfastlines(uint16_t color1, uint16_t color2, Vevor_ST7735* tft)
{
    tft->fillScreen(ST77XX_BLACK);
    for (int16_t y = 0; y < tft->height(); y += 5)
    {
        tft->drawFastHLine(0, y, tft->width(), color1);
    }
    for (int16_t x = 0; x < tft->width(); x += 5)
    {
        tft->drawFastVLine(x, 0, tft->height(), color2);
    }
}

void testdrawrects(uint16_t color, Vevor_ST7735* tft)
{
    tft->fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < tft->width(); x += 6)
    {
        tft->drawRect(tft->width() / 2 - x / 2, tft->height() / 2 - x / 2, x, x, color);
    }
}

void testfillrects(uint16_t color1, uint16_t color2, Vevor_ST7735* tft)
{
    tft->fillScreen(ST77XX_BLACK);
    for (int16_t x = tft->width() - 1; x > 6; x -= 6)
    {
        tft->fillRect(tft->width() / 2 - x / 2, tft->height() / 2 - x / 2, x, x, color1);
        tft->drawRect(tft->width() / 2 - x / 2, tft->height() / 2 - x / 2, x, x, color2);
    }
}

void testfillcircles(uint8_t radius, uint16_t color, Vevor_ST7735* tft)
{
    for (int16_t x = radius; x < tft->width(); x += radius * 2)
    {
        for (int16_t y = radius; y < tft->height(); y += radius * 2)
        {
            tft->fillCircle(x, y, radius, color);
        }
    }
}

void testdrawcircles(uint8_t radius, uint16_t color, Vevor_ST7735* tft)
{
    for (int16_t x = 0; x < tft->width() + radius; x += radius * 2)
    {
        for (int16_t y = 0; y < tft->height() + radius; y += radius * 2)
        {
            tft->drawCircle(x, y, radius, color);
        }
    }
}

void testtriangles(Vevor_ST7735* tft)
{
    tft->fillScreen(ST77XX_BLACK);
    uint16_t color = 0xF800;
    int t;
    int w = tft->width() / 2;
    int x = tft->height() - 1;
    int y = 0;
    int z = tft->width();
    for (t = 0; t <= 15; t++)
    {
        tft->drawTriangle(w, y, y, x, z, x, color);
        x -= 4;
        y += 4;
        z -= 4;
        color += 100;
    }
}

void testroundrects(Vevor_ST7735* tft)
{
    tft->fillScreen(ST77XX_BLACK);
    uint16_t color = 100;
    int i;
    int t;
    for (t = 0; t <= 4; t += 1)
    {
        int x = 0;
        int y = 0;
        int w = tft->width() - 2;
        int h = tft->height() - 2;
        for (i = 0; i <= 16; i += 1)
        {
            tft->drawRoundRect(x, y, w, h, 5, color);
            x += 2;
            y += 3;
            w -= 4;
            h -= 6;
            color += 1100;
        }
        color += 100;
    }
}

void tftPrintTest(Vevor_ST7735* tft)
{
    tft->setTextWrap(false);
    tft->fillScreen(ST77XX_BLACK);
    tft->setCursor(0, 30);
    tft->setTextColor(ST77XX_RED);
    tft->setTextSize(1);
    tft->println("Hello World!");
    tft->setTextColor(ST77XX_YELLOW);
    tft->setTextSize(2);
    tft->println("Hello World!");
    tft->setTextColor(ST77XX_GREEN);
    tft->setTextSize(3);
    tft->println("Hello World!");
    tft->setTextColor(ST77XX_BLUE);
    tft->setTextSize(4);
    tft->print(1234.567);
    delay(1500);
    tft->setCursor(0, 0);
    tft->fillScreen(ST77XX_BLACK);
    tft->setTextColor(ST77XX_WHITE);
    tft->setTextSize(0);
    tft->println("Hello World!");
    tft->setTextSize(1);
    tft->setTextColor(ST77XX_GREEN);
    tft->print(p, 6);
    tft->println(" Want pi?");
    tft->println(" ");
    tft->print(8675309, HEX); // print 8,675,309 out in HEX!
    tft->println(" Print HEX!");
    tft->println(" ");
    tft->setTextColor(ST77XX_WHITE);
    tft->println("Sketch has been");
    tft->println("running for: ");
    tft->setTextColor(ST77XX_MAGENTA);
    tft->print(millis() / 1000);
    tft->setTextColor(ST77XX_WHITE);
    tft->print(" seconds.");
}

void mediabuttons(Vevor_ST7735* tft)
{
    // play
    tft->fillScreen(ST77XX_BLACK);
    tft->fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
    tft->fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
    delay(500);
    // pause
    tft->fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
    tft->fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
    tft->fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
    delay(500);
    // play color
    tft->fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
    delay(50);
    // pause color
    tft->fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
    tft->fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
    // play color
    tft->fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
}

void Vevor_ST7735::demo()
{
    uint16_t time = millis();
    fillScreen(ST77XX_BLACK);
    time = millis() - time;

    Serial.println(time, DEC);
    delay(500);

    // large block of text
    fillScreen(ST77XX_BLACK);
    testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", ST77XX_WHITE, this);
    delay(1000);

    // tft print function!
    tftPrintTest(this);
    delay(4000);

    // a single pixel
    drawPixel(width() / 2, height() / 2, ST77XX_GREEN);
    delay(500);

    // line draw test
    testlines(ST77XX_YELLOW, this);
    delay(500);

    // optimized lines
    testfastlines(ST77XX_RED, ST77XX_BLUE, this);
    delay(500);

    testdrawrects(ST77XX_GREEN, this);
    delay(500);

    testfillrects(ST77XX_YELLOW, ST77XX_MAGENTA, this);
    delay(500);

    fillScreen(ST77XX_BLACK);
    testfillcircles(10, ST77XX_BLUE, this);
    testdrawcircles(10, ST77XX_WHITE, this);
    delay(500);

    testroundrects(this);
    delay(500);

    testtriangles(this);
    delay(500);

    mediabuttons(this);
    delay(500);
}