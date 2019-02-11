#pragma once
#include "Task.h"
#include "States.h"
#include "VirtualQueue.h"
class Logger;
using namespace std;


class Node
{
public:
	const int Id{ id_gen_++ };

	Node() = default;
	Node(int specNum);

	//virtual ~Node();
	double execute(double next_arrival_time, Logger& logger);
	//double kill();

	double get_current_time() const { return current_time_; }
	void set_current_time(double time);
	void set_task(Task&& task);
	Task& get_task();

	double get_left_time() const;
	double get_queue_left_time() const;
	double get_exe_time() const;
	void set_total_exetime();
	double get_total_exetime();
	NodeState get_state() const;

	void enqueue_task(Task& task);
	
	bool try_set_paused_task();
	bool try_set_task_from_queue();
	bool is_arrival_time_reached() const { return arrival_time_reached_; }
	void set_arrival_time_reached(const bool value) { arrival_time_reached_ = value; }

	const NodeSpec& get_spec() const { return spec_; }

	void set_spec(int specNum) { specNum == 1 ? spec_ = NodeSpec::spec1 : NodeSpec::spec2; }

private:
	static int id_gen_;

	NodeSpec spec_;
	double total_exetime_=0;
	//Task* current_task_ptr_ = nullptr;
	Task current_task_;
	double current_time_ = 0;
	double left_time_ = 0;
	double exe_time_ = 0;
	NodeState state_{NodeState::Idle};
	Queue queue_;
	PausedQueue paused_queue_;
	bool arrival_time_reached_ = false;
};

