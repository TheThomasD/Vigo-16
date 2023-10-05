#include "VevorButtons.h"
#include "../log/Logger.h"

VevorButtons::VevorButtons(Timer<> *timer)
{
    clearAll();
    this->timer = timer;
}

void VevorButtons::init()
{
#define COMMAND_BEGIN_BUTTON(BUTTON)                                                \
    b_##BUTTON.begin();                                                             \
    b_##BUTTON.onPressed([this]() { pressed(BT_##BUTTON, Press); });                \
    /*b_##BUTTON.onSequence(2, 750, [this]() { pressed(BT_##BUTTON, DoublePress); });*/ \
    b_##BUTTON.onPressedFor(1500, [this]() { pressed(BT_##BUTTON, LongPress); });   \
    b_##BUTTON.enableInterrupt([this]() { b_##BUTTON.read(); });

    FOREACH_BUTTON(COMMAND_BEGIN_BUTTON)

    timer->every(
        100, [](void *buttons)
        { ((VevorButtons *) buttons)->updateAll();
                    return true; },
        this); // required to receive long press events
}

void VevorButtons::updateAll()
{
#define COMMAND_UPDATE_BUTTON(BUTTON) b_##BUTTON.update();
    FOREACH_BUTTON(COMMAND_UPDATE_BUTTON)
}

bool VevorButtons::isPressed(Button button)
{
#define COMMAN_READ_BUTTON(BUTTON) \
    if (button == BT_##BUTTON)     \
        return b_##BUTTON.read();
    FOREACH_BUTTON(COMMAN_READ_BUTTON)
    return false;
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
    // no logging here! It will kill the interrupt!
    if (callbacks[button][pressType] != nullptr)
    {
        timer->in(
            1, [](void *callback)
            {
                (*(OnButtonCb *) callback)();
                return true;
            },
            &callbacks[button][pressType]);
    }
}