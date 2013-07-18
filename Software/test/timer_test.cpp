#include "timer.h"
#include "gmock/gmock.h"
#include "Arduino.h"

#include "debug.h"
using ::testing::Return;

class TestTimer : public ::testing::Test {
    protected:
    class MockTimerCallback : public Timer::CallbackInterface {
        public:
            virtual ~MockTimerCallback() {}
            MOCK_METHOD0(onTimeout, void());
    };
    ArduinoMock* arduinoMock;
    TestTimer() : arduinoMock( arduinoMockInstance() ) {}
    virtual ~TestTimer() {
        releaseArduinoMock();
    }
};

TEST_F(TestTimer, SimpleTimer)
{
    MockTimerCallback cb;
    EXPECT_CALL(*arduinoMock, millis())
        .Times(2)
        .WillOnce(Return(1))
        .WillOnce(Return(2));
    EXPECT_CALL(cb, onTimeout()).Times(0);
    Timer timer;
    timer.addTimer(cb, 100, false);
    EXPECT_EQ(99, timer.handleTimeouts());
}

TEST_F(TestTimer, SimpleTimeout)
{
    MockTimerCallback cb;
    EXPECT_CALL(*arduinoMock, millis())
        .Times(3)
        .WillOnce(Return(1))
        .WillOnce(Return(2))
        .WillOnce(Return(101))
        ;
    EXPECT_CALL(cb, onTimeout()).Times(1);
    Timer timer;
    timer.addTimer(cb, 100, false);
    EXPECT_EQ(99, timer.handleTimeouts());
    EXPECT_EQ(-1, timer.handleTimeouts());
}

TEST_F(TestTimer, RepeatingTimer)
{
    MockTimerCallback cb;
    EXPECT_CALL(*arduinoMock, millis())
        .Times(3)
        .WillOnce(Return(1))
        .WillOnce(Return(2))
        .WillOnce(Return(101))
        ;
    EXPECT_CALL(cb, onTimeout()).Times(1);
    Timer timer;
    timer.addTimer(cb, 100, true);
    EXPECT_EQ(99, timer.handleTimeouts());
    EXPECT_EQ(100, timer.handleTimeouts());
}

TEST_F(TestTimer, RemoveRepeatingTimer)
{
    MockTimerCallback cb;
    EXPECT_CALL(*arduinoMock, millis())
        .Times(3)
        .WillOnce(Return(1))
        .WillOnce(Return(2))
        .WillOnce(Return(101))
        ;
    EXPECT_CALL(cb, onTimeout()).Times(1);
    Timer timer;
    timer.addTimer(cb, 100, true);
    EXPECT_EQ(99, timer.handleTimeouts());
    EXPECT_EQ(100, timer.handleTimeouts());
    timer.removeTimer(cb);
    EXPECT_EQ(-1, timer.handleTimeouts());
}

TEST_F(TestTimer, TwoTimersAfterEachOther)
{
    MockTimerCallback cb_one;
    MockTimerCallback cb_two;
    EXPECT_CALL(*arduinoMock, millis())
        .Times(4)
        .WillOnce(Return(1))
        .WillOnce(Return(2))
        .WillOnce(Return(3))
        .WillOnce(Return(101))
        ;
    EXPECT_CALL(cb_one, onTimeout()).Times(1);
    EXPECT_CALL(cb_two, onTimeout()).Times(0);
    Timer timer;
    timer.addTimer(cb_one, 100, false);
    timer.addTimer(cb_two, 100, false);
    EXPECT_EQ(98, timer.handleTimeouts());
    EXPECT_EQ(1, timer.handleTimeouts());
}

TEST_F(TestTimer, MainLoop)
{
    boolean g_exitflag = false;
    class MyTimerCallback : public Timer::CallbackInterface {
        public:
            boolean& m_flag;
            MyTimerCallback(boolean& flag) : m_flag(flag) {}

            virtual ~MyTimerCallback() {}
            virtual void onTimeout() {
                m_flag = true;
            }
    };
    MyTimerCallback cb(g_exitflag);
    EXPECT_CALL(*arduinoMock, millis())
        .Times(3)
        .WillOnce(Return(1)) // Add the timer here
        .WillOnce(Return(2)) // First loop, delay 99
        .WillOnce(Return(101)) ; // After delay
    EXPECT_CALL(*arduinoMock, delay(99))
        .Times(1);
    Timer timerHandler;
    timerHandler.addTimer(cb, 100, false);
    int c = 0;
    LOG << "loop()" << std::endl;
    while(!g_exitflag) {
        unsigned long loopdelay = timerHandler.handleTimeouts();
        if(loopdelay == -1) {
            break;
        }
        delay(loopdelay);
        if(c++ > 1) g_exitflag = true;
    }
}

