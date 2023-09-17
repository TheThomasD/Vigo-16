#include "VevorSPI.h"

// SPI Communication (HSPI) for TFT
#define HSPI_SS 15   // Slave Select =CS Chip Select
#define HSPI_MOSI 14 // Master Out Slave In
#define HSPI_MISO 12 // Master In Slave Out
#define HSPI_SCK 13  // Slave Clock =SCL

VevorSPI::VevorSPI() : SPIClass(HSPI)
{
    begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI, HSPI_SS);
}