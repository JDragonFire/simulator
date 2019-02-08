#pragma once
#include <string>
#include <utility>
#include <iomanip>
#include "States.h"
//#include "TaskPool.h"

//#include "Logger.h"

class Task
{
public:
	unsigned long id;
	std::string name;
	int benchmark_num;
	double estimatedTime{0};
	double leftTime{0};
	int benchmark_num;

	Task() : id{ 0 }, name { "undefined" }{}

	Task(unsigned long id, double arrivalTime, int benchmarkNumber) :
		id{id}, arrival_time_{arrivalTime}, benchmark_num{benchmarkNumber}
	{
		
	}

	TaskState get_state() const { return state_; }
	void set_state(const TaskState state) { state_ = state; }
	void set_start_time(const double t) { start_time_ = t; }
	void set_execution_time(const double t) { execution_time_ = t; }
	void set_finished_time(const double t) { finished_time_ = t; }

	void add_diff_to_arrival_time(double t) { arrival_time_ += t; }

	double get_arrival_time() const { return arrival_time_; }
	double get_start_time() const { return start_time_; }
	double get_finished_time() const { return finished_time_; }
	double get_execution_time() const { return execution_time_; }

	int get_executed_core_id() const { return executed_core_id_; }
	void set_executed_core_id(const int id) { executed_core_id_ = id; }

	std::string print();

	// num_tasks
	// cpus
	// ram
	// gpu?
	// per task
	//test

private:
	double arrival_time_;
	double cpu_usage_, mem_usage_, storage_usage;
	TaskState state_{ TaskState::Unexecuted };
	double start_time_{ 0 }, execution_time_{ 0 }, finished_time_{ 0 };
	int executed_core_id_;

	//static std::string get_task_name(int benchmarkNumber)
	//{
	//	static const std::string names[]
	//	{
	//		
	//	};
	//}
};