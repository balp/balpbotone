
#include "Arduino.h"
//#define DEBUG true
#include "engine.h"
#ifdef DEBUG
#include "debug.h"
#endif


Engine left = Engine(3,2,4);
Engine right = Engine(5,6,7);
 
void setup()
{
#ifdef DEBUG
    serial.begin(9600);
    LOG << "setup()" << std::crlf;
#endif
}

void loop()
{
#ifdef DEBUG
    LOG << "loop()" << std::crlf;
#endif
    delay(1000);
    for(int i = 0; i < 4; ++i) {
        // Forward...
#ifdef DEBUG
        LOG << "forward() left" << std::crlf;
#endif
        left.setSpeed(250, false);
        //right.setSpeed(0, false);
        delay(5000);
#ifdef DEBUG
        LOG << "stop() both" << std::crlf;
#endif // DEBUG
        left.setSpeed(0, false);
        right.setSpeed(0, false);
        delay(100);
#if 1
#ifdef DEBUG
        LOG << "reverse() left" << std::crlf;
#endif // DEBUG
        left.setSpeed(250, true);
        delay(1000);
#ifdef DEBUG
        LOG << "stop() both" << std::crlf;
#endif // DEBUG
        left.setSpeed(0, false);
        right.setSpeed(0, false);
        delay(250);
#ifdef DEBUG
        LOG << "forward() right" << std::crlf;
#endif // DEBUG
        //left.setSpeed(0, false);
        right.setSpeed(250, false);
        delay(1000);
#ifdef DEBUG
        LOG << "stop() right" << std::crlf;
#endif // DEBUG
        right.setSpeed(0, true);
        delay(250);
#ifdef DEBUG
        LOG << "reverse() right" << std::crlf;
#endif // DEBUG
        right.setSpeed(250, true);
        delay(1000);
#ifdef DEBUG
        LOG << "stop() both" << std::crlf;
#endif // DEBUG
        left.setSpeed(0, false);
        right.setSpeed(0, false);
        delay(500);
#endif

        //LOG << "backup()" << std::crlf;
        //left.setSpeed(250, true);
        //right.setSpeed(250, true);
        //delay(1000);
        //left.setSpeed(0, false);
        //right.setSpeed(0, false);
        //delay(500);

        //LOG << "left()" << std::crlf;
        // Turn left
        //left.setSpeed(128, true);
        //right.setSpeed(128, false);
        //delay(1000);
        //left.setSpeed(0, false);
        //right.setSpeed(0, false);
        //delay(500);
    }
}

