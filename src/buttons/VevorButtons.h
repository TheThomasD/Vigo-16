#pragma once
#include <EasyButton.h>

#define BUTTON_X_UP 36
#define BUTTON_X_DOWN 39
#define BUTTON_Y_UP 34
#define BUTTON_Y_DOWN 35
#define BUTTON_SET 32
#define BUTTON_Z_UP 25
#define BUTTON_Z_DOWN 26
#define BUTTON_ESC 0

typedef std::function<void(void)> OnButtonCb;

class VevorButtons
{
public:
    enum PressType
    {
        Press,
        DoublePress,
        LongPress
    };
#define FOREACH_BUTTON(COMMAND) \
    COMMAND(BUTTON_X_UP)        \
    COMMAND(BUTTON_X_DOWN)      \
    COMMAND(BUTTON_Y_UP)        \
    COMMAND(BUTTON_Y_DOWN)      \
    COMMAND(BUTTON_SET)         \
    COMMAND(BUTTON_Z_UP)        \
    COMMAND(BUTTON_Z_DOWN)      \
    COMMAND(BUTTON_ESC)
#define COMMAND_ENUM_ENTRY(BUTTON) BT_##BUTTON,
    enum Button
    {
        FOREACH_BUTTON(COMMAND_ENUM_ENTRY)
    };

    void onButton(Button button, PressType pressType, OnButtonCb callback);

    void init();
    void updateAll();
    void clearAll();

protected:
#define COMMAND_CREATE_BUTTON(BUTTON) \
    EasyButton b_##BUTTON = EasyButton(BUTTON);

    FOREACH_BUTTON(COMMAND_CREATE_BUTTON)

    void pressed(Button button, PressType pressType);

    OnButtonCb callbacks[8][3];
};