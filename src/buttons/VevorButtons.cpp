#include "VevorButtons.h"

void VevorButtons::init()
{
#define COMMAND_BEGIN_BUTTON(BUTTON)                                                 \
    b_##BUTTON.begin();                                                              \
    b_##BUTTON.onPressed([this]() { pressed(BT_##BUTTON, Press); });                 \
    b_##BUTTON.onSequence(2, 750, [this]() { pressed(BT_##BUTTON, DoublePress); }); \
    b_##BUTTON.onPressedFor(1500, [this]() { pressed(BT_##BUTTON, LongPress); });    \
    b_##BUTTON.enableInterrupt([this]() { b_##BUTTON.read(); });

    FOREACH_BUTTON(COMMAND_BEGIN_BUTTON)
}

void VevorButtons::updateAll()
{
#define COMMAND_UPDATE_BUTTON(BUTTON) b_##BUTTON.update();
    FOREACH_BUTTON(COMMAND_UPDATE_BUTTON)
}

void VevorButtons::clearAll()
{
    for (uint8_t i = 0; i < 8; i++)
        for (uint8_t j = 0; j < 3; j++)
            callbacks[i][j] = nullptr;
}

void VevorButtons::onButton(Button button, PressType pressType, OnButtonCb callback)
{
    callbacks[button][pressType] = callback;
}

void VevorButtons::pressed(Button button, PressType pressType)
{
    if (callbacks[button][pressType] != nullptr)
        callbacks[button][pressType]();
}