
/*
 *    Calibrate: Every 10ms calibrate ligtsensors
 *    When timer reach 5000ms set state Forward
 *
 *    Forward: Every 100ms check lightsensors
 *    When sensor trigers, record forward time, set state Reverse
 *
 *    Reverse: Run backwards for same time as forward state.
 *    When time passed, set state WaitState
 *
 *    WaitState: do nothing
 *    When timer reaches 5000ms set state Forward
 *
 */

#include "Arduino.h"
//#define DEBUG true
#include "engine.h"
#include "driver.h"
#include "state.h"
#include "timer.h"
#include "lightsensor.h"

#include "debug.h"

Timer timerHandler;

State nullState(NULL);
State calibrateState(NULL);
State forwardState(NULL);
State reverseState(NULL);
State waitState(NULL);
StateMachine stateMachine(nullState);

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

SensorCallback lsensorcb;
SensorCallback rsensorcb;
LightSensor leftSensor = LightSensor(0, &lsensorcb);
LightSensor rightSensor = LightSensor(1, &rsensorcb);

Engine leftEngine = Engine(3,2,4);
Engine rightEngine = Engine(5,6,7);
Driver driver = Driver(&leftEngine, &rightEngine);
 

class Calibrate : public State::CallbackInterface {
    public:
        class EndTimer : public Timer::CallbackInterface {
            public:
                virtual ~EndTimer() {}
                virtual void onTimeout() {
                    stateMachine.setState(forwardState);
                }
        } endTimer;

        class SensorTimer : public Timer::CallbackInterface {
            public:
                virtual ~SensorTimer() {}
                virtual void onTimeout() {
                    leftSensor.takeCalibration();
                    rightSensor.takeCalibration();
                }
        } sensorTimer;

        virtual ~Calibrate() { }
        virtual void onEntry() {
            Serial.println("Calibrate::onEntry()");
            LOG << "Calibrate::onEntry():" << std::crlf;
            timerHandler.addTimer(endTimer, 5000, false);
            timerHandler.addTimer(sensorTimer, 100, true);

        }
        virtual void onExit() {
            LOG << "Calibrate::onExit():" << std::crlf;
            Serial.println("Calibrate::onExit()");
            leftSensor.endCalibration();
            rightSensor.endCalibration();
            timerHandler.removeTimer(sensorTimer);
        }
} calibrateHandler;

class Forward : public State::CallbackInterface {
    public:
        class SensorCb : public LightSensor::LightSensorCallback {
            public:
                virtual ~SensorCb() {}
                virtual void sensorChanged(int value) {
                    stateMachine.setState(reverseState);
                };
        } sensorCallback;

        class SensorTimer : public Timer::CallbackInterface {
            public:
                virtual ~SensorTimer() {}
                virtual void onTimeout() {
                    leftSensor.update();
                    rightSensor.update();
                }
        } sensorTimer;
        virtual ~Forward() {}
        virtual void onEntry() {
            LOG << "Forward::onEntry():" << std::crlf;
            Serial.println("Forward::onEntry()");
            starttime = millis();
            leftSensor.setCallbackInterface(&sensorCallback);
            rightSensor.setCallbackInterface(&sensorCallback);
            timerHandler.addTimer(sensorTimer, 100, true);
            driver.forward(250);
        }
        virtual void onExit() {
            LOG << "Forward::onExit():" << std::crlf;
            Serial.println("Forward::onExit()");
            runtime = millis() - starttime;
            driver.stop();
            leftSensor.setCallbackInterface(&lsensorcb);
            rightSensor.setCallbackInterface(&rsensorcb);
            timerHandler.removeTimer(sensorTimer);
        }
        unsigned long starttime;
        unsigned long runtime;
} forwardHandler;

class Reverse : public State::CallbackInterface {
    public:
        class EndTimer : public Timer::CallbackInterface {
            public:
                virtual ~EndTimer() {}
                virtual void onTimeout() {
                    stateMachine.setState(waitState);
                }
        } endTimer;
        virtual ~Reverse() {}
        virtual void onEntry() {
            Serial.println("Reverse::onEntry()");
            LOG << "Reverse::onEntry():" << std::crlf;
            timerHandler.addTimer(endTimer, forwardHandler.runtime, false);
            driver.reverse(250);
        }
        virtual void onExit() {
            LOG << "Reverse::onExit():" << std::crlf;
            Serial.println("Reverse::onExit()");
            driver.stop();
        }

} reverseHandler;

class Wait : public State::CallbackInterface {
    public:
        class EndTimer : public Timer::CallbackInterface {
            public:
                virtual ~EndTimer() {}
                virtual void onTimeout() {
                    stateMachine.setState(forwardState);
                }
        } endTimer;
        virtual ~Wait() {}
        virtual void onEntry() {
            LOG << "Wait::onEntry():" << std::crlf;
            Serial.println("Wait::onEntry()");
            timerHandler.addTimer(endTimer, 5000, false);
            driver.stop();
        }
        virtual void onExit() {
            LOG << "Wait::onExit():" << std::crlf;
            Serial.println("Wait::onExit()");
        }

} waitHandler;

void setup()
{
#ifdef __AVR__
    Serial.begin(9600);
    Serial.println("setup(): println");
#endif
    LOG << "setup()" << std::crlf;
    lsensorcb.m_name = "left";
    rsensorcb.m_name = "right";
    calibrateState.setCallbackInterface(&calibrateHandler);
    forwardState.setCallbackInterface(&forwardHandler);
    reverseState.setCallbackInterface(&reverseHandler);
    waitState.setCallbackInterface(&waitHandler);

    stateMachine.setState(calibrateState);
    LOG << "setup(): done" << std::crlf;
}

void loop()
{
    unsigned long timeout = timerHandler.handleTimeouts();
    delay(timeout);
}

