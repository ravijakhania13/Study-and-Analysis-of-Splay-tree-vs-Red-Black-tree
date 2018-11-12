#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>

using CLOCK = std::chrono::system_clock;
using us = std::chrono::microseconds;
using ms = std::chrono::milliseconds;
using s = std::chrono::seconds;

class Timer
{
    public:
        // initialize the timer.
        // Postconditions: set the starting and ending event times to 0.
        Timer(): startTime{}, endTime{} {}

        // start timing an event.
        // Postcondition:   record the current time as the starting time
        void start() { startTime = CLOCK::now(); }

        // stop timing an event.
        // Postconditon:    record the current time as the ending time
        void stop() { endTime = CLOCK::now(); }

        // return the time the event took in milliseconds by computing
        // the difference between the ending and starting times
        int duration_in_ms() const {
            return std::chrono::duration_cast<ms>(endTime - startTime).count();
        }

        // return the time the event took in microseconds by computing
        // the difference between the ending and starting times
        int duration_in_us() const {
            return std::chrono::duration_cast<us>(endTime - startTime).count();
        }

        // return the time the event took in seconds by computing
        // the difference between the ending and starting times
        int duration_in_s() const {
            return std::chrono::duration_cast<s>(endTime - startTime).count();
        }

    private:
        CLOCK::time_point startTime, endTime;
};


#endif
