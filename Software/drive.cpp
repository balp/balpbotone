
/*
 *    Calibrate: Every 10ms calibrate ligtsensors
 *    When timer reach 1000ms set state Forward
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

static int debug = 0;
static const int speed = 128;
Timer timerHandler;

State nullState(NULL);
State calibrateState(NULL);
State forwardState(NULL);
State waitReverseState(NULL);
State waitRotateState(NULL);
State reverseState(NULL);
State rotateState(NULL);
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
            leftSensor.resetCalibration();
            rightSensor.resetCalibration();
            timerHandler.addTimer(endTimer, 1000, false);
            timerHandler.addTimer(sensorTimer, 10, true);

        }
        virtual void onExit() {
            LOG << "Calibrate::onExit():" << std::crlf;
            Serial.println("Calibrate::onExit()");
            leftSensor.endCalibration(0.15);
            rightSensor.endCalibration(0.15);
            timerHandler.removeTimer(sensorTimer);
        }
} calibrateHandler;

class Forward : public State::CallbackInterface {
    public:
        class SensorCb : public LightSensor::LightSensorCallback {
            public:
                virtual ~SensorCb() {}
                virtual void sensorChanged(int value) {
                    Serial.print("Forward: ");
                    Serial.print(value);
                    Serial.println(" state change");
                    stateMachine.setState(waitReverseState);
                };
        } sensorCallback;

        class SensorTimer : public Timer::CallbackInterface {
            public:
                virtual ~SensorTimer() {}
                virtual void onTimeout() {
                    if(debug) Serial.println("SensorTimer::onTimeout()");
                    leftSensor.update();
                    rightSensor.update();
                    if(debug) Serial.println("SensorTimer::onTimeout(): exit");
                }
        } sensorTimer;
        virtual ~Forward() {}
        virtual void onEntry() {
            LOG << "Forward::onEntry():" << std::crlf;
            Serial.println("Forward::onEntry()");
            starttime = millis();
            Serial.println(starttime);
            leftSensor.setCallbackInterface(&sensorCallback);
            rightSensor.setCallbackInterface(&sensorCallback);
            Serial.println("Forward::onEntry(): add timer");
            timerHandler.addTimer(sensorTimer, 50, true);
            driver.forward(speed);
            Serial.println("Forward::onEntry(): done");
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
                    stateMachine.setState(waitRotateState);
                }
        } endTimer;
        virtual ~Reverse() {}
        virtual void onEntry() {
            Serial.println("Reverse::onEntry()");
            LOG << "Reverse::onEntry():" << std::crlf;
            timerHandler.addTimer(endTimer, forwardHandler.runtime, false);
            driver.reverse(speed);
        }
        virtual void onExit() {
            LOG << "Reverse::onExit():" << std::crlf;
            Serial.println("Reverse::onExit()");
            driver.stop();
        }

} reverseHandler;

class Rotate : public State::CallbackInterface {
    public:
        class EndTimer : public Timer::CallbackInterface {
            public:
                virtual ~EndTimer() {}
                virtual void onTimeout() {
                    stateMachine.setState(waitState);
                }
        } endTimer;
        virtual ~Rotate() {}
        virtual void onEntry() {
            Serial.println("Rotate::onEntry()");
            LOG << "Rotate::onEntry():" << std::crlf;
            timerHandler.addTimer(endTimer, 5000, false);
            driver.left(speed);
        }
        virtual void onExit() {
            LOG << "Rotate::onExit():" << std::crlf;
            Serial.println("Rotate::onExit()");
            driver.stop();
        }

} rotateHandler;

class Wait : public State::CallbackInterface {
    public:
        class EndTimer : public Timer::CallbackInterface {
            public:
                EndTimer(State& nextState) : m_nextState(nextState) {}
                virtual ~EndTimer() {}
                virtual void onTimeout() {
                    stateMachine.setState(m_nextState);
                }
                State& m_nextState;
        } endTimer;
        Wait(State &nextState) : endTimer(nextState) {};
        virtual ~Wait() {}
        virtual void onEntry() {
            LOG << "Wait::onEntry():" << std::crlf;
            Serial.println("Wait::onEntry()");
            timerHandler.addTimer(endTimer, 250, false);
            driver.stop();
        }
        virtual void onExit() {
            LOG << "Wait::onExit():" << std::crlf;
            Serial.println("Wait::onExit()");
        }

};
Wait reverseWaitHandler(reverseState);
Wait rotateWaitHandler(rotateState);
Wait waitHandler(calibrateState);

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
    waitReverseState.setCallbackInterface(&reverseWaitHandler);
    waitRotateState.setCallbackInterface(&rotateWaitHandler);
    reverseState.setCallbackInterface(&reverseHandler);
    rotateState.setCallbackInterface(&rotateHandler);
    waitState.setCallbackInterface(&waitHandler);

    stateMachine.setState(calibrateState);
    LOG << "setup(): done" << std::crlf;
}

void loop()
{
    int timeout = timerHandler.handleTimeouts();
    if(debug) Serial.print("loop(): delay ");
    if(debug) Serial.println(timeout);
    if(timeout < 0) {
        timeout = 100;
    }
    delay(timeout);
}

