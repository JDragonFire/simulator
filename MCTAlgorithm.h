#pragma once
#include "SchedulerAlgorithm.h"

class MCTAlgorithm : public SchedulerAlgorithm
{
public:
	MCTAlgorithm() : SchedulerAlgorithm("MCT"){}

	void run(std::vector<Host>& cluster, Task& arrivedTask) override
	{
		double minValue = 500000;
		Host* smallest = nullptr;
		for (auto& node : cluster)
		{
			double value = node.get_left_time() + node.get_queue_left_time() + arrivedTask.get_estimated_time(node.get_spec());

			if (value < minValue)
			{
				minValue = value;
				smallest = &node;
			}
		}

		if (smallest == nullptr)
			throw std::invalid_argument("Can't find the smallest");

		smallest->enqueue_task(arrivedTask);
	}
};