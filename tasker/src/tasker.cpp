#include "tasker.hpp"

#include <iostream>
#include <utility>

using namespace std;

Tasker::Tasker() : thread([&] { run(); }), is_terminate(false) {}

Tasker::~Tasker()
{
  is_terminate = true;
  thread.join();
}

void Tasker::run()
{
  while (is_terminate == false)
  {
    cout << this_thread::get_id() << endl;
  }
}
