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
        String x = "-0.00", y = "-0.00", z = "-0.00";
        uint16_t feedRate = 0, spindelRpm = 0;
        bool xEndstop = false, yEndstop = false, zEndstop = false, probe = false;
    };

    const GrblStatus* parse(const char *line); // accept C-string to avoid String copies

protected:
    GrblState parseState(const char *stateField);
    void parseFields(const char *field, GrblStatus &result);
    void parsePositions(const char *field, GrblStatus &result);
    void parseSpeeds(const char *field, GrblStatus &result);
    void parseEndstops(const char *field, GrblStatus &result);

private:
    GrblStatus currentState = {};
};