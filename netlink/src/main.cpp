
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/sysinfo.h>
#include <thread>

#include "pid_tracker.h"

int main()
{
    PID_Tracker tracker;

    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(1));
}
