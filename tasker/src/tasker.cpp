#include "tasker.hpp"

#include <iostream>
#include <utility>

using namespace std;

Tasker::Tasker() : is_terminate(false)
{
	thread = std::move(std::thread(&Tasker::run, this));
}

Tasker::~Tasker()
{
	is_terminate = true;
	thread.join();
}

void Tasker::run()
{
	while(is_terminate == false)
	{
//		this_thread::sleep_for(1s);

		cout << this_thread::get_id() << endl;
	}
}
