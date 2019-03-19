
#include "server.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        cerr << "ERROR: no port provided" << endl;
        return EXIT_FAILURE;
    }

    Server server(std::stoi(argv[1]));

    char c = 'A';

    while(1)
    {
        std::string s(200, c++);
        if(c == '}')
            c = 0;

        server.write(s);
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    return EXIT_SUCCESS;
}
