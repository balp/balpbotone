/*
 *
 */

class State {
    public:
        class CallbackInterface {
            public:
                virtual ~CallbackInterface() {}
                virtual void onEntry() = 0;
        };
        State(CallbackInterface* cb) : m_cb(cb) {}
        void enter(void) {
            m_cb->onEntry();
        }
    private:
        CallbackInterface* m_cb;
};

class StateMachine {
};
