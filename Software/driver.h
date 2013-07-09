/*
 * Robot en driver-
 */

class Driver {
    public:
        Driver(IEngine* left, IEngine* right) : m_left(left), m_right(right) {
            if(m_left) {
                m_left->setSpeed(0,false);
            }
            if(m_right) {
                m_right->setSpeed(0,false);
            }
        }

        void stop() {
            if(m_left) {
                m_left->setSpeed(0,false);
            }
            if(m_right) {
                m_right->setSpeed(0,false);
            }
        }
        void forward(int speed) {
            if(m_left) {
                m_left->setSpeed(speed,false);
            }
            if(m_right) {
                m_right->setSpeed(speed,false);
            }
        }

        void reverse(int speed) {
            if(m_left) {
                m_left->setSpeed(speed,true);
            }
            if(m_right) {
                m_right->setSpeed(speed,true);
            }
        }

        void left(int speed) {
            if(m_left) {
                m_left->setSpeed(speed,true);
            }
            if(m_right) {
                m_right->setSpeed(speed,false);
            }
        }

        void right(int speed) {
            if(m_left) {
                m_left->setSpeed(speed,false);
            }
            if(m_right) {
                m_right->setSpeed(speed,true);
            }
        }

    private:
        IEngine* m_left;
        IEngine* m_right;
};
