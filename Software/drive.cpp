#include "Arduino.h"
#include "engine.h"
#define DEBUG true
#include "debug.h"


Engine left = Engine(3,2,4);
Engine right = Engine(6,5,7);
 
void setup()
{
#ifdef DEBUG
    serial.begin(9600);
#endif
    LOG << "setup()" << std::crlf;
}

void loop()
{
    LOG << "loop()" << std::crlf;
    delay(5000);
    for(int i = 0; i < 4; ++i) {
        // Forward...
        LOG << "forward()" << std::crlf;
        left.setSpeed(250, false);
        right.setSpeed(250, false);
        delay(1000);

        LOG << "left()" << std::crlf;
        // Turn left
        left.setSpeed(128, true);
        right.setSpeed(128, false);
        delay(250);
    }
}

