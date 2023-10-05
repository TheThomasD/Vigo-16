#include "GrblStatusParser.h"
#include "../../log/Logger.h"

// #define DEBUG

const GrblStatusParser::GrblStatus *GrblStatusParser::parse(const String &line)
{
    /* examples:
    <Hold:0|MPos:-1.000,-1.000,-1.000|FS:0,0>
    <Idle|MPos:-1.000,-1.000,-1.000|FS:0,0|WCO:0.000,0.000,0.000>
    <Idle|MPos:-1.000,-1.000,-1.000|FS:0,0|Ov:100,100,100>
    <Idle|MPos:-1.000,-1.000,-1.000|FS:0,0>
    <Alarm|MPos:-1.000,-1.000,-1.000|FS:0,0|Pn:X>
    */
    int currentIndex = 0;
    String currentString;
    while (currentIndex != -1)
    {
        int nextIndex = line.indexOf('|', currentIndex + 1);
        const String field = line.substring(currentIndex + 1, nextIndex > -1 ? nextIndex : line.length() - 2);

#ifdef DEBUG
        log_println("Parsing field: " + field);
#endif
        if (currentIndex == 0)
        {
            currentState.state = parseState(field);
            // have to reset endstop states
            currentState.xEndstop = currentState.yEndstop = currentState.zEndstop = currentState.probe = false;
        }
        else
        {
            parseFields(field, currentState);
        }

        currentIndex = nextIndex;
    }
    return &currentState;
}

void GrblStatusParser::parseFields(const String &field, GrblStatus &result)
{
    switch (field.charAt(0))
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
        log_println("No parser for field: " + field);
#endif
        break;
    }
}

void GrblStatusParser::parsePositions(const String &field, GrblStatus &result)
{
    // example: MPos:0.000,-10.000,5.000
    int firstComma = field.indexOf(',');
    int secondComma = field.indexOf(',', firstComma + 1);
    // cut off last digit to save space
    result.x = field.substring(5, firstComma - 1);
    result.y = field.substring(firstComma + 1, secondComma - 1);
    result.z = field.substring(secondComma + 1, field.length() - 1);
}

void GrblStatusParser::parseSpeeds(const String &field, GrblStatus &result)
{
    // example: FS:500,8000
    int commaIndex = field.indexOf(',');
    result.feedRate = field.substring(3, commaIndex).toInt();
    result.spindelRpm = field.substring(commaIndex + 1).toInt();
}

void GrblStatusParser::parseEndstops(const String &field, GrblStatus &result)
{
    // example: Pn:XYZPDHRS
    result.xEndstop = field.indexOf('X') != -1;
    result.yEndstop = field.indexOf('Y') != -1;
    result.zEndstop = field.indexOf('Z') != -1;
    result.probe = field.indexOf('P', 1) != -1;
}

GrblStatusParser::GrblState GrblStatusParser::parseState(const String &stateField)
{
#define COMMAND_GRBL_STATE_PARSE(STATE) \
    if (stateField.startsWith(#STATE))  \
        return STATE;
    FOREACH_GRBL_STATE(COMMAND_GRBL_STATE_PARSE)
    return Unknown;
}