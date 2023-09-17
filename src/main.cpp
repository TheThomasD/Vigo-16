/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  The 1.14" TFT breakout
  ----> https://www.adafruit.com/product/4383
  The 1.3" TFT breakout
  ----> https://www.adafruit.com/product/4313
  The 1.54" TFT breakout
    ----> https://www.adafruit.com/product/3787
  The 1.69" TFT breakout
    ----> https://www.adafruit.com/product/5206
  The 2.0" TFT breakout
    ----> https://www.adafruit.com/product/4311
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/

#include "Arduino.h"
#include <tft/VevorST7735.h>
#include <audio/VevorSpeaker.h>

VevorST7735 tft = VevorST7735();

void setup(void)
{
  Serial.begin(115200);

  VevorSpeaker::playTone(NOTE_C5, 100);
  VevorSpeaker::playTone(NOTE_E5, 100);
  VevorSpeaker::playTone(NOTE_G5, 100);

  delay(2000);
  Serial.print(F("Hello! ST77xx TFT Test"));

  Serial1.begin(115200);
 
  tft.init();
  delay(2000);

  //tft.demo();

  Serial.println(F("Initialized"));
}

void loop()
{
  if (Serial1.available())
    Serial.write((char) Serial1.read());
  if (Serial.available())
    Serial1.write((char) Serial.read());
  delay(10);
}