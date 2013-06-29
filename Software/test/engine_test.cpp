
#include "engine.h"
#include "gmock/gmock.h"

using ::testing::AnyNumber;
using ::testing::_;

TEST(BasicFunctions, MakeEngineInstane)
{
    ArduinoMock* arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(1,OUTPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(2,OUTPUT)).Times(1);
    EXPECT_CALL(*arduinoMock, pinMode(3,OUTPUT)).Times(1);
    Engine engine(1,2,3);
    releaseArduinoMock();
}

TEST(BasicFunctions, SetSpeed)
{
    ArduinoMock* arduinoMock = arduinoMockInstance();
    EXPECT_CALL(*arduinoMock, pinMode(_,_)).Times(AnyNumber());
    EXPECT_CALL(*arduinoMock, analogWrite(1,128)).Times(1);
    EXPECT_CALL(*arduinoMock, digitalWrite(2,false)).Times(1);
    EXPECT_CALL(*arduinoMock, digitalWrite(3,true)).Times(1);
    Engine engine(1,2,3);
    engine.setSpeed(128, true);
    releaseArduinoMock();
}
