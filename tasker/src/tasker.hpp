//

#pragma once

#include <thread>

class Tasker
{
public:
	Tasker();
	~Tasker();

	void run();

private:
	std::thread thread;
};

