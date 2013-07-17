


/**
 * Keep track of a state.
 */
class State {
    public:
        /**
         * Interface that is called when the state is changed.
         */
        class CallbackInterface {
            public:
                /**
                 * Virtual DTOR
                 */
                virtual ~CallbackInterface() {}

                /**
                 * Called when the state is entered.
                 */
                virtual void onEntry() = 0;

                /**
                 * Called when the state is exited
                 */
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

/**
 * A state machine that handles transitions beteen States.
 */
class StateMachine {
    public:
        /**
         * Constructor this sets the start state to initialState.
         *
         * @param initialState The start state for the machine.
         */
        explicit StateMachine(State& initialState)
            : m_currentState(initialState) {
            initialState.enter();
        }

        /**
         * Change tghe state of the machine.
         *
         * @param state The state t enter.
         */
        void setState(State& state) {
            m_currentState.exit();
            m_currentState = state;
            m_currentState.enter();
        }
    private:
        State& m_currentState;
};
