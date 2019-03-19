
#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    using namespace std;

    int seconds = std::stoi(argv[1]);

    cout << "input:" << seconds << "s" << endl;

    auto start = std::chrono::steady_clock::now();
    auto now   = std::chrono::steady_clock::now();

    while(std::chrono::duration_cast<std::chrono::seconds>(now - start)
          <= std::chrono::seconds(seconds))
    {
        now = std::chrono::steady_clock::now();
    }
}
