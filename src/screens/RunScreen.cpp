#include "RunScreen.h"
#include "../log/Logger.h"

void RunScreen::showHook()
{
    buttons->onButton(VevorButtons::BT_BUTTON_ESC, VevorButtons::LongPress, [this]()
                      { switchScreenCb(AScreen::Files); });
    buttons->onButton(VevorButtons::BT_BUTTON_SET, VevorButtons::Press, [this]()
                      { run(); });

    tft->setTitle("Run");

    tft->setCursor(2, 17);
    tft->setTextColor(ST7735_WHITE);
    tft->print("File: ");
    tft->print(fileToRun.name());
    tft->setCursor(2, 27);
    tft->print("Size: ");
    tft->printf("%.3f KB", (float)fileToRun.size() / 1024.0);

    showProgress();
}

void RunScreen::showProgress()
{
    if (isActive())
    {
        float percentage = (float)processedBytes / (float)fileToRun.size();
        tft->fillRect(0, 70, tft->width(), 30, ST7735_BLACK);
        tft->drawRoundRect(2, 70, tft->width() - 2, 10, 3, ST7735_VEVOR_YELLOW);
        int16_t width = (tft->width() - 4) * percentage;
        tft->fillRoundRect(2, 70, width, 10, 3, ST7735_VEVOR_YELLOW);
        tft->setCursor(tft->width() / 2 - 6, 82);
        tft->setTextColor(ST7735_WHITE);
        tft->printf("%2.1f%%", percentage * 100.0);
        tft->setCursor(2, 91);
        tft->printf("%lu/%lu bytes", processedBytes, fileToRun.size());
    }
}

void RunScreen::run()
{
    if (!running)
    {
        String filename = fileToRun.path();
        fileToRun.close();
        processedBytes = 0;
        SD.begin();
        fileToRun = SD.open(filename);
        running = true;
        timer->every(
            1000, [](void *runScreen)
            { ((RunScreen *)runScreen)->showProgress();
            return ((RunScreen *)runScreen)->running; },
            this);
        timer->every(
            100, [](void *runScreen)
            { return ((RunScreen *)runScreen)->processNextBytes(); },
            this);
    }
}

boolean RunScreen::processNextBytes()
{
#define BUFFER_SIZE 50
    static char buffer[BUFFER_SIZE];
    size_t read = fileToRun.readBytes(&buffer[0], BUFFER_SIZE);
    processedBytes += read;
    running = fileToRun.available() > 0 && isActive();
    return running;
}

void RunScreen::hideHook()
{
    fileToRun.close();
}

void RunScreen::setFile(File file)
{
    fileToRun = file;
    processedBytes = 0;
}