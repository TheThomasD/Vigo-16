// Speaker
#include "Pitches.h"

class Vevor_Speaker
{
public:
    static void playTone(unsigned int frequency, unsigned long duration);

protected:
    static void init();

private:
    static bool initialized;
};