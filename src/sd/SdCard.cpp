#include <Arduino.h>
#include <SD.h>

void printDirectory(File dir, int numTabs)
{
  while (true)
  {

    File entry = dir.openNextFile();
    if (!entry)
    {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++)
    {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory())
    {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    }
    else
    {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.print(entry.size(), DEC);
      time_t lw = entry.getLastWrite();
      struct tm *tmstruct = localtime(&lw);
      Serial.printf("\tLAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
    }
    entry.close();
  }
}

void testSD()
{
  if (!SD.begin())
    Serial.println("Error initializing SD card!");
  else
    Serial.println("SD card OK!");

  Serial.print("Card type: ");
  switch (SD.cardType())
  {
  case CARD_NONE:
    Serial.println("NONE");
    break;
  case CARD_MMC:
    Serial.println("MMC");
    break;
  case CARD_SD:
    Serial.println("SD");
    break;
  case CARD_SDHC:
    Serial.println("SDHC");
    break;
  default:
    Serial.println("Unknown");
  }

  Serial.print("Card size: ");
  Serial.println((float)SD.cardSize() / 1000);

  Serial.print("Total bytes: ");
  Serial.println(SD.totalBytes());

  Serial.print("Used bytes: ");
  Serial.println(SD.usedBytes());

  File dir = SD.open("/");
  printDirectory(dir, 0);
}