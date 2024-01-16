#include "SdCard.h"
#include "../log/Logger.h"

void SdCard::mount()
{
  if (SD.begin())
    mounted = true;
  else
    log_println("Could not mount SD card!");
}

void SdCard::unmount()
{
  SD.end();
  mounted = false;
}

bool SdCard::isMounted()
{
  return mounted;
}

File SdCard::getRoot()
{
  if (!mounted)
    mount();
  return SD.open("/");
}

bool SdCard::ensureMounted()
{
  if (!SD.exists("/"))
  {
    SD.end();
    SD.begin();
  }
  return SD.exists("/");
}

uint64_t SdCard::getTotalSpaceMB()
{
  return SD.totalBytes() / 1024 / 1024;
}

uint64_t SdCard::getUsedSpaceMB()
{
  return SD.usedBytes() / 1024 / 1024;
}

uint16_t SdCard::getNumberOfFiles(File path)
{
  if (!path || !path.isDirectory())
    return 0;
  path.rewindDirectory();
  uint16_t result = 0;
  String s = path.getNextFileName();
  while (s != "")
  {
    result++;
    s = path.getNextFileName();
  }
  return result;
}
