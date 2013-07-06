
#include "Arduino.h"
//#define DEBUG true
#include "engine.h"
#include "driver.h"
#include "debug.h"


Engine left = Engine(3,2,4);
Engine right = Engine(5,6,7);
Driver driver = Driver(&left, &right);
 
void setup()
{
#ifdef __AVR__
    Serial.begin(9600);
#endif
    LOG << "setup()" << std::crlf;
}

void loop()
{
    LOG << "loop()" << std::crlf;
    delay(1000);
    LOG << "Forward()" << std::crlf;
    driver.forward(250);
    delay(1000);
    LOG << "stop()" << std::crlf;
    driver.stop(); delay(250);
    LOG << "reverse()" << std::crlf;
    driver.reverse(250);
    delay(1000);
    LOG << "stop()" << std::crlf;
    driver.stop(); delay(250);
    LOG << "left()" << std::crlf;
    driver.left(250);
    delay(500);
    LOG << "stop()" << std::crlf;
    driver.stop(); delay(250);
    LOG << "right()" << std::crlf;
    driver.right(250);
    delay(500);
    LOG << "stop()" << std::crlf;
    driver.stop(); delay(250);
}

