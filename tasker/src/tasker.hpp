//
//
//

#pragma once

#include <atomic>
#include <thread>

class Tasker
{
public:
  Tasker();
  ~Tasker();

  void run();

private:
  std::thread thread;
  std::atomic<bool> is_terminate;
};
