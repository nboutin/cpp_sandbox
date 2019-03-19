
#pragma once

#include "observer.h"

#include <chrono>
#include <map>

#include <signal.h>
#include <time.h>

class Timer
{
public:
    Timer()
    {
        using namespace std;

        sigemptyset(&set);
        sigaddset(&set, SIGALRM);
        pthread_sigmask(SIG_BLOCK, &set, nullptr);

        struct sigevent sevp;
        sevp.sigev_notify = SIGEV_SIGNAL;
        sevp.sigev_signo  = SIGALRM;
        timer_create(CLOCK_MONOTONIC, &sevp, &timer_id);

        struct itimerspec new_value;
        new_value.it_interval.tv_sec  = 0;
        new_value.it_interval.tv_nsec = std::chrono::nanoseconds(step).count();
        new_value.it_value.tv_sec     = 0;
        new_value.it_value.tv_nsec    = std::chrono::nanoseconds(step).count();
        timer_settime(timer_id, 0, &new_value, nullptr);

        thread t(&Timer::run, this);
        t.detach();
    }

    bool add(std::chrono::milliseconds period, const Observer<bool>& observer)
    {
        using namespace std;
        auto ratio = period / step;    // TODO Add check on bad ratio

        if(timers.find(ratio) == timers.end())
        {
            timers.insert({ratio, timer_data_t()});
        }

        timers[ratio].observable.subscribe(observer);

        return true;
    }

private:
    void run()
    {
        using namespace std;
        while(1)
        {
            int signal;
            auto sig = sigwait(&set, &signal);

            if(signal == SIGALRM)
            {
                last    = current;
                current = std::chrono::high_resolution_clock::now();
                auto duration =
                    std::chrono::duration_cast<std::chrono::microseconds>(current - last).count()
                    - std::chrono::microseconds(step).count();

                cout << "SIGALARM:" << duration << endl;
                for(auto& t : timers)
                {
                    if(++t.second.tick_count == t.first)
                    {
                        t.second.observable.notify(false);
                        t.second.tick_count = 0;
                    }
                }
            }
        }
    }

    struct timer_data_t
    {
        int tick_count = 0;
        Observable<bool> observable;
    };

    const std::chrono::milliseconds step = std::chrono::milliseconds(100);
    sigset_t set;
    timer_t timer_id;
    std::map<int, timer_data_t> timers;

    std::chrono::high_resolution_clock::time_point current, last;
};
