#include "state.h"

#include "gmock/gmock.h"

TEST(TestStates, OnEntry)
{
    class MockStateCallback : public State::CallbackInterface {
        public:
            virtual ~MockStateCallback() {}
            MOCK_METHOD0(onEntry, void());
    } cb;
    State testState(&cb);
    EXPECT_CALL(cb, onEntry()).Times(1);
    testState.enter();
}
