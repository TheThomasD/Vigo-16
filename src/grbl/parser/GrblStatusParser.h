#pragma once

#include <Arduino.h>

class GrblStatusParser
{
public:
#define FOREACH_GRBL_STATE(COMMAND) \
    COMMAND(Idle)                   \
    COMMAND(Run)                    \
    COMMAND(Hold)                   \
    COMMAND(Jog)                    \
    COMMAND(Alarm)                  \
    COMMAND(Door)                   \
    COMMAND(Check)                  \
    COMMAND(Home)                   \
    COMMAND(Sleep)                  \
    COMMAND(Unknown)
#define COMMAND_GRBL_STATE_ENUM(STATE) STATE,
    enum GrblState
    {
        FOREACH_GRBL_STATE(COMMAND_GRBL_STATE_ENUM)
    };

    struct GrblStatus
    {
        GrblState state;
        float x, y, z;
    };

    static GrblStatus parse(const char *buffer);
};