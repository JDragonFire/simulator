#include "Scheduler.h"


Scheduler::Scheduler(SchedulerAlgorithm* algorithm)
{
	algorithm_.reset(algorithm);
}

void Scheduler::run(double currentTime, std::vector<Host>& cluster, Task& arrivedTask) const
{
	algorithm_->run(cluster, arrivedTask);
}

Scheduler& Scheduler::operator=(Scheduler&& other) noexcept
{
	std::swap(algorithm_, other.algorithm_);
	return *this;
}

void Scheduler::change_algorithm(SchedulerAlgorithm* algorithm)
{
	algorithm_.reset(algorithm);
}

//Scheduler& Scheduler::operator=(const Scheduler& other)
//{
//	algorithm_ = 
//}
