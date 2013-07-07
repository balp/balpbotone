
#include "Arduino.h"
//#define DEBUG true
#include "engine.h"
#include "driver.h"
#include "lightsensor.h"

#include "debug.h"

class SensorCallback : public LightSensor::LightSensorCallback {
    public:
        virtual ~SensorCallback() {}
        virtual void sensorChanged(int value) {
            Serial.print(m_name);
            Serial.print(" ");
            Serial.println(value);
        }
        const char* m_name;
};

Engine leftEngine = Engine(3,2,4);
Engine rightEngine = Engine(5,6,7);
SensorCallback lsensorcb;
SensorCallback rsensorcb;
LightSensor leftSensor = LightSensor(0, &lsensorcb);
LightSensor rightSensor = LightSensor(1, &rsensorcb);
Driver driver = Driver(&leftEngine, &rightEngine);
 
void setup()
{
    lsensorcb.m_name = "left";
    rsensorcb.m_name = "right";
#ifdef __AVR__
    Serial.begin(9600);
#endif
    LOG << "setup()" << std::crlf;
    leftSensor.calibrate(100,10);
    rightSensor.calibrate(100,10);
}

void loop()
{
    leftSensor.update();
    rightSensor.update();
#if 0
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
#endif
    delay(100);
}

