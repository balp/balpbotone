/**
 *  Support functions for a timer.
 */

#include "Arduino.h"
/**
 * Yet one more timer class for Arduino.
 *
 * My version is made to work something like this.
 *
 *   #include "Arduino.h"
 *   #include "Timer.h"
 *
 *   class Periodical : public Timer::CallbackInterface {
 *        virtual ~Periodical() {}
 *        virtual void onTimeout() {
 *            // DoStuff...
 *        }
 *   } periodical;
 *
 *   Timer timerHandler;
 *
 *   void setup()
 *   {
 *       timerHandler.addTimer(periodical, 100, true);
 *   }
 *
 *   void loop()
 *   {
 *       unsigned long timeout = timerHandler.handleTimeouts();
 *       delay(timeout);
 *   }
 *
 *
 */
class Timer {
    public:
        class CallbackInterface {
            public:
                virtual ~CallbackInterface() {}
                virtual void onTimeout() = 0;
        };
    private:
        struct TimerInfo {
            TimerInfo(CallbackInterface& cb, unsigned long timeout, bool repeating)
                : callback(cb),
                when(millis() + timeout),
                is_repeating(repeating),
                repeat_timeout(timeout),
                next(NULL)
            {}
            unsigned long when;
            CallbackInterface& callback;
            bool is_repeating;
            unsigned long repeat_timeout;
            struct TimerInfo* next;
        } *head;
    public:
        /**
         * Default constructor.
         */
        Timer() : head(NULL) {}

        /**
         * Add a new timer.
         *
         * @param callback the function onTimeout() in this class will be called
         * @param timeout the number of milliseconds to the timeout
         * @param repeating if true the callback will be called every timeout milliseconds.
         */
        void addTimer(CallbackInterface& callback, int timeout, bool repeating) {
            TimerInfo* info = new TimerInfo(callback, timeout, repeating);
            insertTimer(info);
        }

        /**
         * Called to handle the timeouts.
         *
         * The functions is designed to be part of a main loop, like this.
         *   
         *   void loop(void) {
         *      unsigned long timeout = timerHandler->handleTimeouts();
         *      delay(timeout);
         *   }
         *
         *
         * @return the number of milliseconds to next timer or -1 if no active timers.
         */
        int handleTimeouts() {
            if(! head) {
                return -1;
            }
            unsigned long now = millis();
            if(now >= head->when) {
                head->callback.onTimeout();
                TimerInfo* tmp = head;
                head = head->next;
                if(tmp->is_repeating) {
                    tmp->when += tmp->repeat_timeout;
                    insertTimer(tmp);
                } else {
                    delete head;
                }
            }
            if(head) {
                return head->when - now;
            } else {
                return -1;
            }
        }

    private:
        void insertTimer(TimerInfo* info) {
            if(! head) { // No events queued
                head = info;
            } else {
                TimerInfo* tmp = head;
                TimerInfo* prev = NULL;
                while(tmp) {
                    if(tmp->when > info->when) {
                        info->next = tmp;
                        if(prev) {
                            prev->next = info;
                        } else {
                            head = info;
                        }
                        tmp = NULL;
                    } else {
                        if(tmp->next) {
                            prev = tmp;
                            tmp = tmp->next;
                        } else {
                            tmp->next = info;
                            tmp = NULL;
                        }
                    }
                }
            }
        }

};
