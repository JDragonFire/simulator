#include <iostream>
#include <sstream>
#include "../includes/utils.h"
#include "../includes/queue.h"
#include "../includes/cluster.h"
#include "../includes/scenario.h"
#include "../includes/cluster_simulation.h"
#include "../includes/job.h"
#include "../dependencies/spdlog/spdlog.h"

//#include "spdlog/spdlog.h"
#include "../dependencies/spdlog/sinks/basic_file_sink.h"
#include "../dependencies/spdlog/common.h"
#include <fstream>

//#include "dependencies/

namespace ClusterSimulator
{
	std::shared_ptr<spdlog::logger> ClusterSimulation::file_logger = spdlog::basic_logger_mt("file_logger", "log_output.txt");
	std::ofstream ClusterSimulation::jobmart_file_;
	std::ofstream performance_("performance_.txt");
	bprinter::TablePrinter ClusterSimulation::tp_{ &jobmart_file_ };

	ClusterSimulation::ClusterSimulation(Scenario& scenario, Cluster& cluster)
		: cluster_{ cluster }, scenario_{ scenario }, all_queues_{ scenario.generate_queues(*this) },
		dispatcher_{ this }
	{
		cluster.simulation = this;

		// TODO: set default queue

		current_time_ = scenario.initial_time_point;
		std::sort(all_queues_.begin(), all_queues_.end());

		log_action_ = [this]
		{
			int total_using_slots = 0;
			for (const auto& q : this->all_queues_)
				total_using_slots += q.using_job_slots();

			using_slot_record_.insert_or_assign(this->get_current_time(), total_using_slots);

			if (!scenario_.is_empty() || total_using_slots != 0)
				this->after_delay(this->logging_frequency, this->log_action_, 2);
		};
		after_delay(logging_frequency, log_action_, 2);
	
		

		//set algoritms
		for(auto& q : this->all_queues_)
			q.set_algorithm(QueueAlgorithms::MCT);

		reserve_dispatch_event();

		file_logger->set_pattern("[%l] %v");
		initialise_tp();
		std::cout << "Simulation start!" << std::endl;
	}

	Queue& ClusterSimulation::find_queue(const std::string& name)
	{
		auto it = all_queues_.begin();
		it = std::find_if(it, all_queues_.end(),
		             [&name](Queue& queue) { return queue.name == name; });

		if (it == all_queues_.end())
			throw std::out_of_range("Can't find a queue of name " + name + " in this simulation.");

		return *it;
	}

	const Host& ClusterSimulation::find_host(const std::string& name) const
	{
		auto it = cluster_.find_node(name);
		if (it == cluster_.end())
			throw std::out_of_range("Can't find a host of name " + name + " in this cluster.");

		return it->second;
	}

	ClusterSimulation::EventItem::EventItem(const ScenarioEntry& entry, ClusterSimulation& simulation)
	{
		priority = 0;
		time = entry.timestamp;

		if (entry.type == ScenarioEntry::ScenarioEntryType::SUBMISSION)
		{
			action = [&simulation, entry]
			{
				if (entry.event_detail.queue_name == "-" || entry.event_detail.queue_name.empty())
					return;

				Queue& queue = simulation.find_queue(entry.event_detail.queue_name);
			
				queue.enqueue(Job{ entry, queue, simulation.get_current_time() });

				//queue.dispatch();
				simulation.reserve_dispatch_event();
				
				simulation.num_submitted_jobs_++;
			};
		}
		else if (entry.type == ScenarioEntry::ScenarioEntryType::CHANGE_STATUS)
		{
			action = [&simulation, &entry]
			{
				if (entry.event_detail.host_name.empty())
					return;

				Host& host = simulation.get_cluster().find_node(entry.event_detail.host_name)->second;

				host.set_status(entry.event_detail.host_status);

				Utils::enum_const_ref_holder<HostStatus> test = Utils::enum_to_string<HostStatus>(host.status);
				std::stringstream ss;
				ss << test;

				log(LogLevel::info, "Host {0}'s status is changed to {1}", host.id, ss.str());
			};
		}
		else
		{
			throw std::logic_error("Not implemented;");
		}
	}


	/**
	 * 
	 */
	void ClusterSimulation::after_delay(std::chrono::milliseconds delay, Action block, int priority)
	{
		events_.push(EventItem{current_time_ + delay, std::move(block), priority});
		//spdlog::debug("Event is added ");
	}

	// No use
	bool ClusterSimulation::run(ms run_time)
	{
		//while (!events_.empty() && current_time_ <= run_time)
		//	next();

		std::cout << "Simulation finished. " << std::endl;
		return true;
	}

	bool ClusterSimulation::run()
	{
		while (true)
		{
			if (!scenario_.is_empty())
			{
				std::vector<ScenarioEntry> next_entries;
				ms next_arrival_time;
				std::tie(next_entries, next_arrival_time) = scenario_.pop_all_latest();

				auto next_event_time = events_.top().time;
				while (next_event_time <= next_arrival_time)
				{
					next();
					next_event_time = events_.top().time;
				}

				for (const auto& entry : next_entries)
				{
					events_.push(EventItem(entry, *this));
				}
			}
			else
			{
				while (!events_.empty())
					next();
				break;	
			}
			
		}
		return true;
	}

	void ClusterSimulation::print_summary() const
	{
		auto total_simulation_time = latest_finish_time_.time_since_epoch().count();
		//double total_cpu_power{ 0.0 };
		int num_total_available_hosts{ 0 };
		int num_total_applications{ scenario_.num_unique_apps() };
		for (const auto& [name, host] : cluster_)
		{
			if (!host.is_available_at_least_once)
				continue;

			num_total_available_hosts++;
			//total_cpu_power += 
		}

		std::string algorithm_name;
		if (const auto algorithm = all_queues_.front().current_algorithm; algorithm != nullptr)
			algorithm_name = algorithm->get_name();
		else
			algorithm_name = "Scenario";

		std::stringstream ss;

		ss << "\n" <<
			"## Simulation Summary" << "\n" <<
			"### Simulated duration: " << total_simulation_time << "\n" <<
			"### Available hosts: " << num_total_available_hosts << "\n" <<
			"### Number of applications: " << num_total_applications << "\n" <<
			"### Number of submitted jobs: " << num_submitted_jobs_ << "\n" <<
			"#### Queue algorithm: " << algorithm_name << "\n" <<
			//"### Number of successful jobs: " << num_successful_jobs_ << "\n" <<
			"#### Dispatch frequency: " << dispatch_frequency.count() << " ms\n" <<
			"#### Logging frequency: " << logging_frequency.count() << " ms\n" <<
			std::endl;


		log(LogLevel::info, ss.str());

		performance_<< "MakeSpan\n" <<
			"### Simulated duration: " << total_simulation_time << "\n" <<
			"### Number of submitted jobs: " << num_submitted_jobs_ << "\n" <<	
			std::endl;
		
		
		performance_ << " end : "<< using_slot_record_.size() << "\n";
		// for (const slot_record_entry s : using_slot_record_) performance_ << " time : "<< s.time_stamp.time_since_epoch().count() << ", value : "<< s.value << "\n";
		
		for (auto [time, count] : using_slot_record_)
			performance_ << " time : "<< time.time_since_epoch().count() << ", value : "<< count << "\n";
	}

	void ClusterSimulation::next()
	{
		const auto event_item = events_.top();
		events_.pop(); 

		if (current_time_ != event_item.time /*&& event_item.priority < 2*/)
		{
			current_time_ = event_item.time;
			if (event_item.priority < 2)
			{
				log(LogLevel::info, "Current Time: {0}", current_time_.time_since_epoch().count());
				update_latest_finish_time(current_time_);
			}
		}
		event_item.action();

		//log(LogLevel::warn, "Time: {0}, Priority: {1}", event_item.time.time_since_epoch().count(), event_item.priority);
	}

	void ClusterSimulation::reserve_dispatch_event()
	{
		if (next_dispatch_reserved)
			return;

		after_delay(
			Utils::get_time_left_until_next_period(current_time_, dispatch_frequency),
			std::ref(dispatcher_), 1);

		next_dispatch_reserved = true;
	}

	/**
	 * Initialise the table header of jobmart_raw_replica file.
	 */
	void ClusterSimulation::initialise_tp()
	{
		jobmart_file_.open("jobmart_raw_replica.txt");
		//tp_.AddColumn("submit_time_gmt", 10);
		//tp_.AddColumn("start_time_gmt", 10);
		//tp_.AddColumn("finish_time_gmt", 10);
		//tp_.AddColumn("submit_time", 10);
		tp_.AddColumn("start_time", 10);
		tp_.AddColumn("finish_time", 10);
		tp_.AddColumn("queue_name", 20);
		tp_.AddColumn("exec_hostname", 10);
		tp_.AddColumn("num_exec_procs", 10);
		tp_.AddColumn("num_slots", 10);
		tp_.AddColumn("job_exit_status", 10);
		tp_.AddColumn("application_name", 20);
		tp_.AddColumn("job_id", 5);
		tp_.AddColumn("job_pend_time", 10);
		tp_.AddColumn("job_run_time", 10);
		tp_.AddColumn("job_mem_usage", 10);
		tp_.AddColumn("job_swap_usage", 10);
		tp_.AddColumn("job_cpu_time", 10);
		tp_.PrintHeader();
	}
}

