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

// SPI Communication (HSPI) for TFT
#define HSPI_SS    15 // Slave Select =CS Chip Select
#define HSPI_MOSI  14 // Master Out Slave In
#define HSPI_MISO  12 // Master In Slave Out
#define HSPI_SCK   13 // Slave Clock =SCL

// Speaker
#define SPEAKER 21

// Buttons
#define BUTTON_X_UP   36
#define BUTTON_X_DOWN 39
#define BUTTON_Y_UP   34
#define BUTTON_Y_DOWN 35
#define BUTTON_SET    32
#define BUTTON_Z_UP   25
#define BUTTON_Z_DOWN 26
#define BUTTON_ESC     0

#define FOREACH_BUTTON(COMMAND)  \
  COMMAND(BUTTON_X_UP) \
  COMMAND(BUTTON_X_DOWN) \
  COMMAND(BUTTON_Y_UP) \
  COMMAND(BUTTON_Y_DOWN) \
  COMMAND(BUTTON_SET) \
  COMMAND(BUTTON_Z_UP) \
  COMMAND(BUTTON_Z_DOWN) \
  COMMAND(BUTTON_ESC)

#define CREATE_BUTTON(BUTTON) \
  EasyButton b_##BUTTON(BUTTON); \
  void shortPress_##BUTTON(){shortPress(BUTTON);}; \
  void longPress_##BUTTON(){longPress(BUTTON);}; \
  void doublePress_##BUTTON(){doublePress(BUTTON);}; \
  void interrupt_##BUTTON(){b_##BUTTON.read();};

#define INIT_BUTTON(BUTTON) \
  b_##BUTTON.begin(); \
  b_##BUTTON.onPressed(shortPress_##BUTTON); \
  b_##BUTTON.onPressedFor(2000, longPress_##BUTTON); \
  b_##BUTTON.onSequence(2, 1500, doublePress_##BUTTON); \
  b_##BUTTON.enableInterrupt(interrupt_##BUTTON);

#define UPDATE_BUTTON(BUTTON) \
  b_##BUTTON.update();

#define BUTTON_TO_STRING(BUTTON) \
  if (button == BUTTON) return #BUTTON;

#define CREATE_BUTTON_TO_STRING \
  String buttonToString(uint8_t button) { \
    FOREACH_BUTTON(BUTTON_TO_STRING) \
    return "UNKOWN"; \
  }

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
