#pragma once
#include "VirtualQueue.h"
#include "Logger.h"
#include "TaskPool.h"


VirtualQueue::~VirtualQueue() = default;

void VirtualQueue::enqueue(Task& task) 
{
	task.estimatedTime = getEstimateTime(task);
	task.leftTime = task.estimatedTime;

	list_.push_back(task);
	printAssign(task);
}

Task VirtualQueue::dequeue()
{
	Task task = list_.front();
	list_.erase(list_.begin());
	return task;
}

double VirtualQueue::get_time_left() const
{
	double sumLeft = 0;

	for (const auto& i : list_)
		sumLeft += i.leftTime;

	return sumLeft;
}

