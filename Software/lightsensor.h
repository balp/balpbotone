/*
 *
 */
#include "Arduino.h"
#include "debug.h"



class LightSensor {
    public:
    /**
     * This callback interface will be used when the light
     * passes below or above the threshold.
     */
    class LightSensorCallback {
        public:
            virtual ~LightSensorCallback() {}
            virtual void sensorChanged(int value) = 0;
    };
    /**
     * Make a new light senensor.
     *
     * @param pin The Analog pin that the light sensor in connected to.
     */
    LightSensor(int pin, LightSensorCallback* cb) : m_min(1024), m_max(0), m_pin(pin), m_cb(cb) { };

    void calibrate(int loops, int loopdelay) {
        for(int i = 0 ; i < loops; ++i) {
            int value = analogRead(m_pin);
            if(value < m_min) {
                m_min = value;
            }
            if(value > m_max) {
                m_max = value;
            }
            LOG << "Calibrate " << i << " min " << m_min << " max " << m_max << std::endl;

            delay(loopdelay);
        }
        m_min *= 0.90;
        m_max *= 1.10;
    }

    int getValue() {
        return analogRead(m_pin);
    }

    void update() {
        int currentValue = getValue();
        LOG << "update " << currentValue << std::endl;
        if(currentValue < m_min || currentValue > m_max ) {
            if(m_cb) {
                m_cb->sensorChanged(currentValue);
            }
        }
    }
    private:
    int m_min;
    int m_max;
    int m_pin;
    LightSensorCallback* m_cb;
};
