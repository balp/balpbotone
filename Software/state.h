/*
 *
 */

class State {
    public:
        class CallbackInterface {
            public:
                virtual ~CallbackInterface() {}
                virtual void onEntry() = 0;
                virtual void onExit() = 0;
        };
        State(CallbackInterface* cb) : m_cb(cb) {}
        void enter(void) const {
            m_cb->onEntry();
        }
        void exit(void) const {
            m_cb->onExit();
        }
    private:
        CallbackInterface* m_cb;
};

class StateMachine {
    public:
        explicit StateMachine(State& initialState)
            : m_currentState(initialState) {
            initialState.enter();
        }
        void setState(State& state) {
            m_currentState.exit();
            m_currentState = state;
            m_currentState.enter();
        }
    private:
        State& m_currentState;
};
