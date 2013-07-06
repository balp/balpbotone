#ifndef ENGINE_H
#define ENGINE_H
#include "Arduino.h"

#ifdef DEBUG
#include "debug.h"
#endif //DEBUG
class IEngine {
    public:
        ~IEngine() {}
        virtual void setSpeed(int speed, bool reverse) = 0;
};

class Engine : public IEngine {
    int m_enable;
    int m_one;
    int m_two;
    public:
    Engine(int enablepin, int onepin, int twopin) :
        m_enable(enablepin),
        m_one(onepin),
        m_two(twopin)
    {
        pinMode(m_enable, OUTPUT);
        pinMode(m_one, OUTPUT);
        pinMode(m_two, OUTPUT);
    }

    virtual ~Engine() {}

    virtual void setSpeed(int speed, bool reverse) {
#ifdef DEBUG
        LOG << "setSpeed( " << speed << ", " << reverse << ")" << std::crlf
            << m_enable << " = " << speed << std::crlf
            << m_one << " = !" << reverse << std::crlf
            << m_two << " = " << reverse << std::crlf;
#endif //DEBUG
        analogWrite(m_enable, speed);
        digitalWrite(m_one, !reverse);
        digitalWrite(m_two, reverse);
    }
};
#endif // ENGINE_H
