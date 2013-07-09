
#include "engine.h"
#include "EngineMock.h"

#include "driver.h"
#include "gmock/gmock.h"

using ::testing::Mock;

TEST(TestDriverInit, MakeInstance)
{
    MockIEngine lengine;
    MockIEngine rengine;
    EXPECT_CALL(lengine, setSpeed(0, false));
    EXPECT_CALL(rengine, setSpeed(0, false));
    Driver driver_dut(&lengine, &rengine);
}

class TestDriver : public ::testing::Test {
    protected:
        MockIEngine lengine;
        MockIEngine rengine;
        Driver* driver_dut;

        TestDriver() {
            EXPECT_CALL(lengine, setSpeed(0, false));
            EXPECT_CALL(rengine, setSpeed(0, false));
            driver_dut = new Driver(&lengine, &rengine);
            Mock::VerifyAndClearExpectations(&lengine);
            Mock::VerifyAndClearExpectations(&rengine);
        }
};

TEST_F(TestDriver, stop)
{
    EXPECT_CALL(lengine, setSpeed(0, false));
    EXPECT_CALL(rengine, setSpeed(0, false));
    driver_dut->stop();
}

TEST_F(TestDriver, Forward_255)
{
    EXPECT_CALL(lengine, setSpeed(255, false));
    EXPECT_CALL(rengine, setSpeed(255, false));
    driver_dut->forward(255);
}

TEST_F(TestDriver, reverse_240)
{
    EXPECT_CALL(lengine, setSpeed(240, true));
    EXPECT_CALL(rengine, setSpeed(240, true));
    driver_dut->reverse(240);
}

TEST_F(TestDriver, TurnLeft)
{
    EXPECT_CALL(lengine, setSpeed(250, true));
    EXPECT_CALL(rengine, setSpeed(250, false));
    driver_dut->left(250);
}

TEST_F(TestDriver, TurnRight)
{
    EXPECT_CALL(lengine, setSpeed(250, false));
    EXPECT_CALL(rengine, setSpeed(250, true));
    driver_dut->right(250);
}
