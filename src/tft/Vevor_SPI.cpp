#include "Vevor_SPI.h"

Vevor_SPI::Vevor_SPI() : SPIClass(HSPI) {
    begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI, HSPI_SS);
}