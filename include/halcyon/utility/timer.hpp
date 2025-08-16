#pragma once

#include <chrono>
#include <iosfwd>

// utility/timer.hpp:
// Timing facilities.

namespace hal
{
    // A simple timer for all your timing needs. It'll keep running no matter what.
    class timer
    {
    public:
        using clock = std::chrono::steady_clock;

        // Create a timer that begins at the time of construction.
        timer();

        void reset();

        double operator()() const;

        template <typename Rep, typename Period>
        timer& operator=(std::chrono::duration<Rep, Period> dur)
        {
            reset();
            m_epoch -= dur;
            return *this;
        }

        template <typename Rep, typename Period>
        timer& operator+=(std::chrono::duration<Rep, Period> dur)
        {
            m_epoch -= dur;
            return *this;
        }

        template <typename Rep, typename Period>
        timer& operator-=(std::chrono::duration<Rep, Period> dur)
        {
            m_epoch += dur;
            return *this;
        }

        // Get the `timer::clock::duration` that's elapsed since the timer's epoch.
        // For a convenience function that returns the time in seconds as a `double`,
        // see `timer::operator()()`.
        clock::duration elapsed() const;

        // Get the `timer::clock::time point` at which the timer started.
        clock::time_point epoch() const;

    private:
        clock::time_point m_epoch;
    };

    std::ostream& operator<<(std::ostream& str, timer t);
}
