#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

#define EXTERNAL_NUM_INTERRUPTS 16
#define NUM_DIGITAL_PINS        40
#define NUM_ANALOG_INPUTS       16

#define analogInputToDigitalPin(p)  (((p)<20)?(analogChannelToDigitalPin(p)):-1)
#define digitalPinToInterrupt(p)    (((p)<40)?(p):-1)
#define digitalPinHasPWM(p)         (p < 34)

// NOTE: these are not pin numbers, but GPIO numbers!

static const uint8_t TX = 1;
static const uint8_t RX = 3;

// I2C Communication
static const uint8_t SDA = 21;
static const uint8_t SCL = 22;

// SPI Communication (VSPI) for SD card (defaults for VSPI)
static const uint8_t SS    =  5; // Slave Select =CS Chip Select
static const uint8_t MOSI  = 23; // Master Out Slave In
static const uint8_t MISO  = 19; // Master In Slave Out
static const uint8_t SCK   = 18; // Slave Clock =SCL

// SPI Communication (HSPI) for TFT (defaults for HSPI)
static const uint8_t HSPI_SS    = 15; // Slave Select =CS Chip Select
static const uint8_t HSPI_MOSI  = 14; // Master Out Slave In
static const uint8_t HSPI_MISO  = 12; // Master In Slave Out
static const uint8_t HSPI_SCK   = 13; // Slave Clock =SCL

// speaker
static const uint8_t SPEAKER = 21;

/*
static const uint8_t A0 = 36;
static const uint8_t A3 = 39;
static const uint8_t A4 = 32;
static const uint8_t A5 = 33;
static const uint8_t A6 = 34;
static const uint8_t A7 = 35;
static const uint8_t A10 = 4;
static const uint8_t A11 = 0;
static const uint8_t A12 = 2;
static const uint8_t A13 = 15;
static const uint8_t A14 = 13;
static const uint8_t A15 = 12;
static const uint8_t A16 = 14;
static const uint8_t A17 = 27;
static const uint8_t A18 = 25;
static const uint8_t A19 = 26;

static const uint8_t T0 = 4;
static const uint8_t T1 = 0;
static const uint8_t T2 = 2;
static const uint8_t T3 = 15;
static const uint8_t T4 = 13;
static const uint8_t T5 = 12;
static const uint8_t T6 = 14;
static const uint8_t T7 = 27;
static const uint8_t T8 = 33;
static const uint8_t T9 = 32;

static const uint8_t DAC1 = 25;
static const uint8_t DAC2 = 26;
*/

#endif /* Pins_Arduino_h */
