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
        GrblState state = Alarm;
        float x = 0, y = 0, z = 0;
        uint16_t feedRate = 0, spindelRpm = 0;
        bool xEndstop = false, yEndstop = false, zEndstop = false, probe = false;
    };

    GrblStatus parse(const String line);

protected:
    GrblState parseState(const String stateField);
    void parseFields(const String field, GrblStatus *result);
    void parsePositions(const String field, GrblStatus *result);
    void parseSpeeds(const String field, GrblStatus *result);
    void parseEndstops(const String field, GrblStatus *result);
};