//
//
//

#pragma once

#include <thread>
#include <atomic>

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

