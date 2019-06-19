#pragma once
#include "SchedulerAlgorithm.h"
#include "RandomAlgorithm.h"

class Scheduler
{
public:
	Scheduler() : algorithm_{new RandomAlgorithm}{}

	explicit Scheduler(SchedulerAlgorithm* algorithm);

	void run(double currentTime, std::vector<Host>& cluster, Task& arrivedTask) const;

	const std::string& get_algorithm_name() const { return algorithm_->algorithm_name; }
	const bool is_algorithm_using_complete_signal();

	Scheduler& operator=(Scheduler&&) noexcept;
	//Scheduler& operator=(const Scheduler&);

	void change_algorithm(SchedulerAlgorithm* algorithm);

private:
	std::unique_ptr<SchedulerAlgorithm> algorithm_;
};

