#include "VevorST7735.h"

#define TFT_CS 15
#define TFT_RST 2
#define TFT_DC 12

// TFT SPI (HSPI)
VevorSPI VevorST7735::tftSpi = VevorSPI();

VevorST7735::VevorST7735() : Adafruit_ST7735(&tftSpi, TFT_CS, TFT_DC, TFT_RST){};

void VevorST7735::init()
{
    initR(INITR_18BLACKTAB);
    setColRowStart(2, 1);
    setRotation(1);
}

void VevorST7735::setStaStatus(const Status status)
{
    staStatus = status;
    redrawStatus();
}

void VevorST7735::setApStatus(const Status status, uint8_t numberOfClients)
{
    apStatus = status;
    apClients = numberOfClients;
    redrawStatus();
}

void VevorST7735::setSerialStatus(const Status status)
{
    serialStatus = status;
    redrawStatus();
}

uint16_t VevorST7735::getColor(const Status status)
{
    switch (status)
    {
    case CONNECTED:
        return ST7735_GREEN;
    case DISCONNECTED:
        return ST7735_RED;
    default:
        return ST7735_ORANGE;
    }
}

void VevorST7735::redrawStatus()
{
    const uint8_t statusWidth = 3 * 6 + 1;
    fillRect(0, 0, 60, 10, ST7735_BLACK);
    fillRect(0, 0, statusWidth, 10, getColor(staStatus));
    fillRect(statusWidth + 1, 0, statusWidth, 10, getColor(apStatus));
    fillRect(2 * statusWidth + 2, 0, statusWidth, 10, getColor(serialStatus));

    setTextSize(1);
    setTextColor(ST7735_BLACK);

    setCursor(1, 1);
    print("STA");
    setCursor(statusWidth + 1 + 1, 1);
    print("AP");
    print(apClients);
    setCursor(2 * statusWidth + 2 + 1, 1);
    print("SER");
}

void VevorST7735::clear()
{
    fillScreen(ST7735_BLACK);
    redrawStatus();
}

/*

float p = 3.1415926;

void testlines(uint16_t color, VevorST7735 *tft)
{
    fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < width(); x += 6)
    {
        drawLine(0, 0, x, height() - 1, color);
        delay(0);
    }
    for (int16_t y = 0; y < height(); y += 6)
    {
        drawLine(0, 0, width() - 1, y, color);
        delay(0);
    }

    fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < width(); x += 6)
    {
        drawLine(width() - 1, 0, x, height() - 1, color);
        delay(0);
    }
    for (int16_t y = 0; y < height(); y += 6)
    {
        drawLine(width() - 1, 0, 0, y, color);
        delay(0);
    }

    fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < width(); x += 6)
    {
        drawLine(0, height() - 1, x, 0, color);
        delay(0);
    }
    for (int16_t y = 0; y < height(); y += 6)
    {
        drawLine(0, height() - 1, width() - 1, y, color);
        delay(0);
    }

    fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < width(); x += 6)
    {
        drawLine(width() - 1, height() - 1, x, 0, color);
        delay(0);
    }
    for (int16_t y = 0; y < height(); y += 6)
    {
        drawLine(width() - 1, height() - 1, 0, y, color);
        delay(0);
    }
}

void testdrawtext(char *text, uint16_t color, VevorST7735 *tft)
{
    setCursor(0, 0);
    setTextColor(color);
    setTextWrap(true);
    print(text);
}

void testfastlines(uint16_t color1, uint16_t color2, VevorST7735 *tft)
{
    fillScreen(ST77XX_BLACK);
    for (int16_t y = 0; y < height(); y += 5)
    {
        drawFastHLine(0, y, width(), color1);
    }
    for (int16_t x = 0; x < width(); x += 5)
    {
        drawFastVLine(x, 0, height(), color2);
    }
}

void testdrawrects(uint16_t color, VevorST7735 *tft)
{
    fillScreen(ST77XX_BLACK);
    for (int16_t x = 0; x < width(); x += 6)
    {
        drawRect(width() / 2 - x / 2, height() / 2 - x / 2, x, x, color);
    }
}

void testfillrects(uint16_t color1, uint16_t color2, VevorST7735 *tft)
{
    fillScreen(ST77XX_BLACK);
    for (int16_t x = width() - 1; x > 6; x -= 6)
    {
        fillRect(width() / 2 - x / 2, height() / 2 - x / 2, x, x, color1);
        drawRect(width() / 2 - x / 2, height() / 2 - x / 2, x, x, color2);
    }
}

void testfillcircles(uint8_t radius, uint16_t color, VevorST7735 *tft)
{
    for (int16_t x = radius; x < width(); x += radius * 2)
    {
        for (int16_t y = radius; y < height(); y += radius * 2)
        {
            fillCircle(x, y, radius, color);
        }
    }
}

void testdrawcircles(uint8_t radius, uint16_t color, VevorST7735 *tft)
{
    for (int16_t x = 0; x < width() + radius; x += radius * 2)
    {
        for (int16_t y = 0; y < height() + radius; y += radius * 2)
        {
            drawCircle(x, y, radius, color);
        }
    }
}

void testtriangles(VevorST7735 *tft)
{
    fillScreen(ST77XX_BLACK);
    uint16_t color = 0xF800;
    int t;
    int w = width() / 2;
    int x = height() - 1;
    int y = 0;
    int z = width();
    for (t = 0; t <= 15; t++)
    {
        drawTriangle(w, y, y, x, z, x, color);
        x -= 4;
        y += 4;
        z -= 4;
        color += 100;
    }
}

void testroundrects(VevorST7735 *tft)
{
    fillScreen(ST77XX_BLACK);
    uint16_t color = 100;
    int i;
    int t;
    for (t = 0; t <= 4; t += 1)
    {
        int x = 0;
        int y = 0;
        int w = width() - 2;
        int h = height() - 2;
        for (i = 0; i <= 16; i += 1)
        {
            drawRoundRect(x, y, w, h, 5, color);
            x += 2;
            y += 3;
            w -= 4;
            h -= 6;
            color += 1100;
        }
        color += 100;
    }
}

void tftPrintTest(VevorST7735 *tft)
{
    setTextWrap(false);
    fillScreen(ST77XX_BLACK);
    setCursor(0, 30);
    setTextColor(ST77XX_RED);
    setTextSize(1);
    println("Hello World!");
    setTextColor(ST77XX_YELLOW);
    setTextSize(2);
    println("Hello World!");
    setTextColor(ST77XX_GREEN);
    setTextSize(3);
    println("Hello World!");
    setTextColor(ST77XX_BLUE);
    setTextSize(4);
    print(1234.567);
    delay(1500);
    setCursor(0, 0);
    fillScreen(ST77XX_BLACK);
    setTextColor(ST77XX_WHITE);
    setTextSize(0);
    println("Hello World!");
    setTextSize(1);
    setTextColor(ST77XX_GREEN);
    print(p, 6);
    println(" Want pi?");
    println(" ");
    print(8675309, HEX); // print 8,675,309 out in HEX!
    println(" Print HEX!");
    println(" ");
    setTextColor(ST77XX_WHITE);
    println("Sketch has been");
    println("running for: ");
    setTextColor(ST77XX_MAGENTA);
    print(millis() / 1000);
    setTextColor(ST77XX_WHITE);
    print(" seconds.");
}

void mediabuttons(VevorST7735 *tft)
{
    // play
    fillScreen(ST77XX_BLACK);
    fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
    fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
    delay(500);
    // pause
    fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
    fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
    fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
    delay(500);
    // play color
    fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
    delay(50);
    // pause color
    fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
    fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
    // play color
    fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
}

void VevorST7735::demo()
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
*/