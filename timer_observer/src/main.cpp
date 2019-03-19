
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <thread>

#include "measure.h"
#include "timer.h"

int main()
{
    using namespace std;
    using namespace std::chrono;

    Timer timer;

    Measure m("1s");
    Measure m2("A 100ms");
    Measure m3("400ms");
    Measure m4("B 100ms");

    timer.add(chrono::seconds(1), m);
    timer.add(chrono::milliseconds(100), m2);
    timer.add(chrono::milliseconds(400), m3);
    timer.add(chrono::milliseconds(100), m4);

    this_thread::sleep_for(seconds(20));
}
