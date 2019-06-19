#include "Host.h"
#include "Simulation.h"
#include "Job.h"
#include "dependencies/spdlog/spdlog.h"

namespace ClusterSimulator
{
	int Host::id_gen_ = 0;
	void Host::execute_job(const Job& job)
	{
		if (slot_running_ + job.slot_required > max_slot)
			spdlog::error("Host {0}: Slot required for job {1} cannot be fulfilled with this host.", id, job.id);

		slot_running_ += job.slot_required;
	}

	void Host::exit_job(const Job& job)
	{
		slot_running_ -= job.slot_required;
	}
}


using namespace std;

int Host::id_gen_ = 0;

Host::Host(int specNum) : spec_{ specNum } {}

NodeState Host::get_state() const
{
	return state_;
}

void Host::enqueue_task(Task& task)
{
	task.set_executed_core_id(Id);
	queue_.enqueue(task);
}

bool Host::try_set_paused_task()
{
	if (paused_queue_.is_empty())
		return false;
	set_task(paused_queue_.dequeue());
	return true;
}

bool Host::try_set_task_from_queue()
{
	if (queue_.is_empty())
		return false;
	set_task(queue_.dequeue());
	return true;
}


double Host:: execute(double next_arrival_time, Logger& logger)
{
	Task& task = current_task_;

	//상태 변환하는 부분
	if (state_ == NodeState::Idle) {
		//할 일 없음.
		return 0;
	}
	
	if (task.get_state() == TaskState::Unexecuted) 
	{
		// Renew current time
		if (current_time_ < task.get_arrival_time() && task.get_arrival_time() < next_arrival_time)
			set_current_time(task.get_arrival_time());

		//한 번 실행됬다가 남은 작업을 실행할 때 상태 전환
		//실행 시킬 수 있다면 실행상태로.
		/*
		cpu[core].startTime = cpuNowTime[core];
		cpu[core].state = Execute;
		printResume(cpu[core]);
		*/
		task.set_start_time(get_current_time());
		task.set_state(TaskState::Running);
	}
	else if (task.get_state() == TaskState::Running)
	{
		;
	}

	if (next_arrival_time < current_time_ + task.leftTime) {
		//다음 도착, 종료예정시간까지 실행이 불가능하다면
		//다음 도착, 종료예정시간까지만 실행
		task.leftTime -= next_arrival_time - current_time_;
		task.set_execution_time(task.get_execution_time() + next_arrival_time - current_time_);
		current_time_ = next_arrival_time;
		task.set_finished_time(current_time_+ task.leftTime);

		logger.writeLog(task);
		//printf("4 : %f\n",now);
		//cpu[core].state = Execute;
	}
	else {
		//남은 시간 내에 완수한다면
		//실행하고 유휴상태로 전환
		current_time_ += task.leftTime;
		//cout << "5 : " << cpuNowTime[core] << endl;
		task.set_execution_time(task.get_execution_time() + task.leftTime);
		set_total_exetime();
		task.leftTime = 0;
		task.set_finished_time(current_time_);
		task.set_state(TaskState::Completed);
		//printCom(cpu[core]);
		logger.writeLog(task);
		state_ = NodeState::Idle;
	}

	return task.get_execution_time();
}

void Host:: set_current_time(const double time)
{
	current_time_ = time;
}

void Host::set_task(Task&& task)
{
	//current_task_ptr_ = &task;
	current_task_ = task;
	//current_task_.set_state(TaskState::Unexecuted);
	//state_ = task.get_state();
	state_ = NodeState::Running;
	//set_current_time(task.get_arrival_time());
}
Task& Host:: get_task()
{	
	// Unsafe
	return current_task_;
}
double Host::get_left_time() const
{
	if (state_ != NodeState::Idle) 
	{
		return left_time_;
	}
	else 
	{
		return 0;
	}

}

double Host::get_queue_left_time() const
{
	return queue_.get_time_left() + paused_queue_.get_time_left();
}

double Host::get_exe_time() const
{
	if (state_ != NodeState::Idle) {
		return exe_time_;
	}
	else {
		return 0;
	}

}

void Host::set_total_exetime()
{
	total_exetime_ = total_exetime_ + exe_time_;
}

double Host::get_total_exetime()
{
	return total_exetime_;
}


