// main.cpp

#include <iostream>
using namespace std;

#include <mutex>
#include <thread>

uint32_t value = 0;
mutex mut_value;

void incremente()
{
  // Simulate long calculus
  this_thread::sleep_for(1s);

  lock_guard<mutex> guard(mut_value);
  value++;
}

int main(int argc, char** argv)
{
  thread t1(incremente);
  thread t2(incremente);

  t1.join();
  t2.join();

  cout << "value = " << value << endl;
}
