#include <Arduino.h>
#include <EasyButton.h>

// Buttons
#define BUTTON_X_UP 36
#define BUTTON_X_DOWN 39
#define BUTTON_Y_UP 34
#define BUTTON_Y_DOWN 35
#define BUTTON_SET 32
#define BUTTON_Z_UP 25
#define BUTTON_Z_DOWN 26
#define BUTTON_ESC 0

#define FOREACH_BUTTON(COMMAND) \
  COMMAND(BUTTON_X_UP)          \
  COMMAND(BUTTON_X_DOWN)        \
  COMMAND(BUTTON_Y_UP)          \
  COMMAND(BUTTON_Y_DOWN)        \
  COMMAND(BUTTON_SET)           \
  COMMAND(BUTTON_Z_UP)          \
  COMMAND(BUTTON_Z_DOWN)        \
  COMMAND(BUTTON_ESC)

#define CREATE_BUTTON(BUTTON)                           \
  EasyButton b_##BUTTON(BUTTON);                        \
  void shortPress_##BUTTON() { shortPress(BUTTON); };   \
  void longPress_##BUTTON() { longPress(BUTTON); };     \
  void doublePress_##BUTTON() { doublePress(BUTTON); }; \
  void interrupt_##BUTTON() { b_##BUTTON.read(); };

#define INIT_BUTTON(BUTTON)                             \
  b_##BUTTON.begin();                                   \
  b_##BUTTON.onPressed(shortPress_##BUTTON);            \
  b_##BUTTON.onPressedFor(2000, longPress_##BUTTON);    \
  b_##BUTTON.onSequence(2, 1500, doublePress_##BUTTON); \
  b_##BUTTON.enableInterrupt(interrupt_##BUTTON);

#define UPDATE_BUTTON(BUTTON) \
  b_##BUTTON.update();

#define BUTTON_TO_STRING(BUTTON) \
  if (button == BUTTON)          \
    return #BUTTON;

#define CREATE_BUTTON_TO_STRING         \
  String buttonToString(uint8_t button) \
  {                                     \
    FOREACH_BUTTON(BUTTON_TO_STRING)    \
    return "UNKOWN";                    \
  }

CREATE_BUTTON_TO_STRING

void shortPress(uint8_t button)
{
  Serial.print("Short press: ");
  Serial.println(buttonToString(button));
}

void longPress(uint8_t button)
{
  Serial.print("Long press: ");
  Serial.println(buttonToString(button));
}

void doublePress(uint8_t button)
{
  Serial.print("Double press: ");
  Serial.println(buttonToString(button));
}

FOREACH_BUTTON(CREATE_BUTTON)

void init()
{
  FOREACH_BUTTON(INIT_BUTTON)
}

void loopButtons()
{
  FOREACH_BUTTON(UPDATE_BUTTON)
}