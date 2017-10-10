#include "tasker.hpp"

#include <utility>

using namespace std;

Tasker::Tasker()
{
	thread = std::move(std::thread(&Tasker::run, this));
}

Tasker::~Tasker()
{
	thread.join();
}

void Tasker::run()
{

}
