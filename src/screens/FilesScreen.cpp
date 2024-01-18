#include "FilesScreen.h"

#define NUMBER_OF_FILES 9

void FilesScreen::showHook()
{
    buttons->onButton(VevorButtons::BT_BUTTON_ESC, VevorButtons::LongPress, [this]()
                      {
                          switchScreenCb(AScreen::Menu); //
                      });
    buttons->onButton(VevorButtons::BT_BUTTON_Y_UP, VevorButtons::Press, [this]()
                      {
                          if (currentPathPosition > 0)
                          {
                              currentPathPosition--;
                              showFiles();
                          } //
                      });
    buttons->onButton(VevorButtons::BT_BUTTON_Y_DOWN, VevorButtons::Press, [this]()
                      {
                          if (currentPathPosition < sdCard->getNumberOfFiles(currentPath) - 1)
                          {
                              currentPathPosition++;
                              showFiles();
                          } //
                      });
    buttons->onButton(VevorButtons::BT_BUTTON_SET, VevorButtons::Press, [this]()
                      {
                          String fileName = String(currentFile.name());
                          if (currentFile.isDirectory())
                          {
                              currentPath = currentFile;
                              currentFile = File();
                              currentPathPosition = 0;
                              showFiles();
                          }
                          else if (fileName.endsWith(".nc") || fileName.endsWith(".ngc"))
                          {
                              setFileToRunCb(currentFile);
                              switchScreenCb(AScreen::Run);
                          } //
                      });
    buttons->onButton(VevorButtons::BT_BUTTON_ESC, VevorButtons::Press, [this]()
                      {
                          String path = currentPath.path();
                          if (path != "/")
                          {
                              currentPath.close();
                              currentPathPosition = 0;
                              currentPath = SD.open(path.substring(0, path.lastIndexOf("/")));
                              if (!currentPath)
                                  currentPath = sdCard->getRoot();
                              showFiles();
                          } //
                      });

    tft->setTitle("Files");

    currentPath = sdCard->getRoot();
    currentPathPosition = 0;
    showFiles();
}

void FilesScreen::showFiles()
{
    int16_t yIndex = 18;
    tft->fillRect(0, yIndex, tft->width(), tft->height() - yIndex, ST7735_BLACK);
    bool canShow = sdCard->ensureMounted();
    if (canShow)
    {
        if (wasDisconnected)
        {
            currentPath = sdCard->getRoot();
            wasDisconnected = false;
        }
        currentPath.rewindDirectory();
        tft->setCursor(2, yIndex);
        tft->setTextColor(ST7735_WHITE);
        tft->print(currentPath.path());
        yIndex += 10;
        int16_t numberOfEntries = currentPathPosition;
        for (int i = 0; i < currentPathPosition; i++)
            currentPath.openNextFile().close();
        currentFile.close();
        currentFile = File();
        for (int i = 0; true; i++)
        {
            tft->setCursor(2, yIndex);
            File file = currentPath.openNextFile();
            if (file)
            {
                numberOfEntries++;
                if (i < NUMBER_OF_FILES)
                {
                    if (i == 0)
                    {
                        tft->fillRect(0, yIndex - 1, tft->width(), 10, ST7735_VEVOR_YELLOW);
                        tft->setTextColor(ST7735_BLACK);
                    }
                    else
                        tft->setTextColor(ST7735_WHITE);
                    tft->print(file.isDirectory() ? " D " : "   ");
                    tft->print(file.name());
                    yIndex += 10;
                }
                if (!currentFile)
                    currentFile = file;
                else
                    file.close();
            }
            else
                break;
        }

        tft->setTextColor(ST7735_WHITE);
        tft->setCursor(2, tft->height() - 9);
        if (numberOfEntries == 0)
            tft->printf("No files");
        else
            tft->printf("File %d/%d", currentPathPosition + 1, numberOfEntries);
        if (currentFile.isDirectory() || String(currentFile.name()).endsWith(".nc"))
        {
#define OPEN_WIDTH (4 * 6 + 2)
            tft->fillRect(tft->width() - OPEN_WIDTH, tft->height() - 10, OPEN_WIDTH, 10, ST7735_VEVOR_YELLOW);
            tft->setTextColor(ST7735_BLACK);
            tft->setCursor(tft->width() - OPEN_WIDTH + 1, tft->height() - 9);
            tft->print("Open");
        }
    }
    else
    {
        wasDisconnected = true;
        tft->setCursor(2, yIndex);
        tft->setTextColor(ST7735_WHITE);
        tft->print("SD not mountable! ESC to remount...");
    }
}

void FilesScreen::hideHook()
{
    sdCard->unmount();
}