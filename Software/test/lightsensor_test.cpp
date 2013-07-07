/*
 *
 */

#include "lightsensor.h"
#include "gmock/gmock.h"

using ::testing::Return;
using ::testing::_;

TEST(TestLightSensorInit, MakeInstance)
{
    LightSensor sensor(0, NULL);
}

class TestLightSensor : public ::testing::Test {
    protected:
        ArduinoMock* arduinoMock;
        TestLightSensor() : arduinoMock(arduinoMockInstance()) {
        }
        virtual ~TestLightSensor() {
            releaseArduinoMock();
        }
};

TEST_F(TestLightSensor, ReadValue)
{
    EXPECT_CALL(*arduinoMock, analogRead(0)).Times(1).WillOnce(Return(256));

    LightSensor sensor(0, NULL);
    EXPECT_EQ(256, sensor.getValue());
}

TEST_F(TestLightSensor, CalibrateOne)
{
    EXPECT_CALL(*arduinoMock, analogRead(0)).Times(1).WillOnce(Return(256));
    EXPECT_CALL(*arduinoMock, delay(10)).Times(1);
    LightSensor sensor(0, NULL);
    sensor.calibrate(1, 10);
}

TEST_F(TestLightSensor, CalibrateLonger)
{
    EXPECT_CALL(*arduinoMock, analogRead(0))
        .Times(4)
        .WillOnce(Return(250))
        .WillOnce(Return(240))
        .WillOnce(Return(260))
        .WillOnce(Return(250)) ;
    EXPECT_CALL(*arduinoMock, delay(10)).Times(4);
    LightSensor sensor(0, NULL);
    sensor.calibrate(4, 10);
}

TEST_F(TestLightSensor, ChangeDownCallback)
{
    class MockLightSensorCallback : public LightSensor::LightSensorCallback {
        public:
            virtual ~MockLightSensorCallback() {}
            MOCK_METHOD1(sensorChanged, void(int value));
    } cb;
    EXPECT_CALL(*arduinoMock, analogRead(0))
        .Times(5)
        .WillOnce(Return(250))
        .WillOnce(Return(240))
        .WillOnce(Return(260))
        .WillOnce(Return(250)) 
        .WillOnce(Return(200)) ;
    EXPECT_CALL(cb, sensorChanged(200)).Times(1);
    EXPECT_CALL(*arduinoMock, delay(10)).Times(4);
    LightSensor sensor(0, &cb);
    sensor.calibrate(4, 10);
    sensor.update();
}


TEST_F(TestLightSensor, ChangeToSmall)
{
    class MockLightSensorCallback : public LightSensor::LightSensorCallback {
        public:
            virtual ~MockLightSensorCallback() {}
            MOCK_METHOD1(sensorChanged, void(int value));
    } cb;
    EXPECT_CALL(*arduinoMock, analogRead(0))
        .Times(5)
        .WillOnce(Return(250))
        .WillOnce(Return(240))
        .WillOnce(Return(260))
        .WillOnce(Return(250)) 
        .WillOnce(Return(240)) ;
    EXPECT_CALL(cb, sensorChanged(_)).Times(0);
    EXPECT_CALL(*arduinoMock, delay(10)).Times(4);
    LightSensor sensor(0, &cb);
    sensor.calibrate(4, 10);
    sensor.update();
}

TEST_F(TestLightSensor, ChangeUpHigher)
{
    class MockLightSensorCallback : public LightSensor::LightSensorCallback {
        public:
            virtual ~MockLightSensorCallback() {}
            MOCK_METHOD1(sensorChanged, void(int value));
    } cb;
    EXPECT_CALL(*arduinoMock, analogRead(0))
        .Times(5)
        .WillOnce(Return(250))
        .WillOnce(Return(240))
        .WillOnce(Return(260))
        .WillOnce(Return(250)) 
        .WillOnce(Return(300)) ;
    EXPECT_CALL(cb, sensorChanged(300)).Times(1);
    EXPECT_CALL(*arduinoMock, delay(10)).Times(4);
    LightSensor sensor(0, &cb);
    sensor.calibrate(4, 10);
    sensor.update();
}

TEST_F(TestLightSensor, CalibrateTo500)
{
    class MockLightSensorCallback : public LightSensor::LightSensorCallback {
        public:
            virtual ~MockLightSensorCallback() {}
            MOCK_METHOD1(sensorChanged, void(int value));
    } cb;
    EXPECT_CALL(*arduinoMock, analogRead(0))
        .Times(5)
        .WillOnce(Return(500))
        .WillOnce(Return(490))
        .WillOnce(Return(510))
        .WillOnce(Return(500)) 
        .WillOnce(Return(250)) ;
    EXPECT_CALL(cb, sensorChanged(250)).Times(1);
    EXPECT_CALL(*arduinoMock, delay(10)).Times(4);
    LightSensor sensor(0, &cb);
    sensor.calibrate(4, 10);
    sensor.update();
}

