#pragma once
#include "old/Task.h"
#include "old/States.h"
#include "old/VirtualQueue.h"
#include "EnumConverter.h"
class Logger;

class Host
{
public:
	const int Id{ id_gen_++ };

	Host() = default;
	Host(int specNum);

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

namespace ClusterSimulator
{
	class Job;

	enum class HostStatus
	{
		OK,
		Closed_Adm,
		Closed_Busy,
		Closed_Excl,
		Closed_cu_Excl,
		Closed_Full,
		Closed_LIM,
		Closed_Lock,
		Closed_Wind,
		Closed_EGO,
		Unavail,
		Unreach
	};

	class Host
	{
	public:
		const std::string name;
		const int cpu_factor;
		const int ncpus;
		const int nprocs;
		const int ncores;
		const int nthreads;
		const int max_slot;
		const int max_mem;
		const int max_swp;
		const int max_tmp;
		const int id{ id_gen_++ };

		HostStatus status;
		int num_current_jobs;
		int num_current_running_slots;
		bool is_available_at_least_once;

		// Status mutator methods
		void execute_job(const Job& job);
		void exit_job(const Job& job);
		void set_status(HostStatus value) 
		{
			status = value;
			if (!is_available_at_least_once && value == HostStatus::OK)
				is_available_at_least_once = true;
		}

		// Initialise Host from status data.
		Host(const std::string name, int cpu_factor, int ncpus, int nprocs, int ncores, int nthreads, int max_slot, int max_mem, int max_swp,
			int max_tmp, HostStatus status)
			: name(name),
			  cpu_factor(cpu_factor),
			  ncpus(ncpus),
			  nprocs(nprocs),
			  ncores(ncores),
			  nthreads(nthreads),
			  max_slot(max_slot),
			  max_mem(max_mem),
			  max_swp(max_swp),
			  max_tmp(max_tmp),
			  status(status)
		{
			if (status == HostStatus::OK)
				is_available_at_least_once = true;
		}

	private:
		int slot_running_{ 0 };

		static int id_gen_;
	};
}


template<> char const* Utils::enum_strings<ClusterSimulator::HostStatus>::data[] = {
	"OK", 
	"Closed_Adm",
	"Closed_Busy",
	"Closed_Excl",
	"Closed_cu_Excl",
	"Closed_Full",
	"Closed_LIM",
	"Closed_Lock",
	"Closed_Wind",
	"Closed_EGO",
	"Unavail",
	"Unreach"
};

