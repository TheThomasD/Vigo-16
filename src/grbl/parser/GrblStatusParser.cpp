#include "GrblStatusParser.h"
#include "../../log/Logger.h"
#include <cstring>

// #define DEBUG

const GrblStatusParser::GrblStatus *GrblStatusParser::parse(const char *line)
{
    /* examples:
    <Hold:0|MPos:-1.000,-1.000,-1.000|FS:0,0>
    <Idle|MPos:-1.000,-1.000,-1.000|FS:0,0|WCO:0.000,0.000,0.000>
    <Idle|MPos:-1.000,-1.000,-1.000|FS:0,0|Ov:100,100,100>
    <Idle|MPos:-1.000,-1.000,-1.000|FS:0,0>
    <Alarm|MPos:-1.000,-1.000,-1.000|FS:0,0|Pn:X>
    */
    const char *p = line;
    // start after initial '<' if present
    if (*p == '<') p++;

    bool firstField = true;
    while (*p && *p != '>')
    {
        const char *sep = strchr(p, '|');
        const char *end = sep ? sep : strchr(p, '>');
        if (!end) end = p + strlen(p);
        size_t len = end - p;

        // copy field into a small stack buffer (fields are short)
        char fieldBuf[128];
        if (len >= sizeof(fieldBuf)) len = sizeof(fieldBuf) - 1;
        memcpy(fieldBuf, p, len);
        fieldBuf[len] = '\0';

#ifdef DEBUG
        log_println(String("Parsing field: ") + fieldBuf);
#endif

        if (firstField)
        {
            // parse state (C-string)
            currentState.state = parseState(fieldBuf);
            // reset endstop states
            currentState.xEndstop = currentState.yEndstop = currentState.zEndstop = currentState.probe = false;
            firstField = false;
        }
        else
        {
            parseFields(fieldBuf, currentState);
        }

        if (!sep)
            break;
        p = sep + 1;
    }

    return &currentState;
}

void GrblStatusParser::parseFields(const char *field, GrblStatus &result)
{
    switch (field[0])
    {
    case 'M':
        parsePositions(field, result);
        break;

    case 'F':
        parseSpeeds(field, result);
        break;

    case 'P':
        parseEndstops(field, result);
        break;

    default:
#ifdef DEBUG
        log_print("No parser for field: ");
        log_println(field);
#endif
        break;
    }
}

void GrblStatusParser::parsePositions(const char *field, GrblStatus &result)
{
    // example: MPos:0.000,-10.000,5.000
    const char* str = field + 5;  // skip "MPos:"
    char buffer[12];  // Increased size for safety (e.g., "-123.456" = 8 chars + null)
    
    // Parse X (until first comma)
    int i = 0;
    while (str[i] && str[i] != ',' && i < sizeof(buffer) - 1) {
        buffer[i] = str[i];
        i++;
    }
    buffer[i] = '\0';  // Proper null termination AFTER last character
    result.x = buffer;
    
    str += i + 1;  // move past comma
    
    // Parse Y (until second comma)
    i = 0;
    while (str[i] && str[i] != ',' && i < sizeof(buffer) - 1) {
        buffer[i] = str[i];
        i++;
    }
    buffer[i] = '\0';  // Proper null termination
    result.y = buffer;
    
    str += i + 1;  // move past comma
    
    // Parse Z (until end of field)
    i = 0;
    while (str[i] && str[i] != '|' && str[i] != '\0' && i < sizeof(buffer) - 1) {
        buffer[i] = str[i];
        i++;
    }
    buffer[i] = '\0';  // Proper null termination
    result.z = buffer;
}

void GrblStatusParser::parseSpeeds(const char *field, GrblStatus &result)
{
    // example: FS:500,8000
    const char* str = field + 3;  // skip "FS:"
    
    // Parse feedRate until comma
    uint16_t feedRate = 0;
    while (*str && *str != ',') {
        feedRate = feedRate * 10 + (*str - '0');
        str++;
    }
    result.feedRate = feedRate;
    
    // Parse spindelRpm after comma
    if (*str == ',') str++; //skip comma
    uint16_t spindelRpm = 0;
    while (*str && *str != '\0') {
        spindelRpm = spindelRpm * 10 + (*str - '0');
        str++;
    }
    result.spindelRpm = spindelRpm;
}

void GrblStatusParser::parseEndstops(const char *field, GrblStatus &result)
{
    // example: Pn:XYZPDHRS
    const char* str = field;
    result.xEndstop = false;
    result.yEndstop = false;
    result.zEndstop = false;
    result.probe = false;
    
    for (int i = 3; str[i]; i++) {  // start after "Pn:"
        if (str[i] == 'X') result.xEndstop = true;
        else if (str[i] == 'Y') result.yEndstop = true;
        else if (str[i] == 'Z') result.zEndstop = true;
        else if (str[i] == 'P') result.probe = true;
    }
}

GrblStatusParser::GrblState GrblStatusParser::parseState(const char *stateField)
{
#define COMMAND_GRBL_STATE_PARSE(STATE) \
    if (strncmp(stateField, #STATE, sizeof(#STATE)-1) == 0) \
        return STATE;
    FOREACH_GRBL_STATE(COMMAND_GRBL_STATE_PARSE)
    return Unknown;
}