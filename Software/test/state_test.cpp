#include "state.h"

#include "gmock/gmock.h"

TEST(TestStates, OnEntry)
{
    class MockStateCallback : public State::CallbackInterface {
        public:
            virtual ~MockStateCallback() {}
            MOCK_METHOD0(onEntry, void());
            MOCK_METHOD0(onExit, void());
    } cb;
    State testState(&cb);
    EXPECT_CALL(cb, onEntry()).Times(1);
    testState.enter();
}

class TestStateHandler : public ::testing::Test {
    protected:
    class MockStateCallback : public State::CallbackInterface {
        public:
            virtual ~MockStateCallback() {}
            MOCK_METHOD0(onEntry, void());
            MOCK_METHOD0(onExit, void());
    } cb;
    State firstState;
    State secondState;

    TestStateHandler() : firstState(&cb), secondState(&cb) {}
};

TEST_F(TestStateHandler, InitialState)
{
    EXPECT_CALL(cb, onEntry()).Times(1);
    StateMachine sm(firstState);
}

TEST_F(TestStateHandler, changeState)
{
    EXPECT_CALL(cb, onEntry()).Times(2);
    EXPECT_CALL(cb, onExit()).Times(1);

    StateMachine sm(firstState);
    sm.setState(secondState);
}
