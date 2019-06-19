#pragma once
//#include "Queue.h"
//#include "Scenario.h"
#include "Cluster.h"
#include <functional>
#include <chrono>
#include <queue>
#include "Queue.h"
#include "dependencies/spdlog/sinks/basic_file_sink.h"

namespace ClusterSimulator
{
	class Scenario;
	using ms = std::chrono::time_point<std::chrono::milliseconds>;

	class ClusterSimulation
	{

	public:
		using Action = std::function<void()>;
		struct EventItem
		{
			ms time;
			Action action;

			EventItem(ms time, Action action) : time{ time }, action{ std::move(action) } {};
			EventItem(const ScenarioEntry& entry, ClusterSimulation& simulation);

			bool operator<(const EventItem& a) const { return a.time < this->time; }
		};

	private:
		void next();
		ms current_time_;
		std::priority_queue<EventItem> events_;


	public:
		ClusterSimulation(Scenario& scenario, Cluster& cluster);
		~ClusterSimulation();

		// TODO: use id instead of name
		Queue& find_queue(const std::string& name);
		Host& find_host(const std::string& name) const;

		std::size_t event_count() const { return events_.size(); }
		//void log(const std::string& s) const { std::cout << current_time_ << " " << s << std::endl; }
		void after_delay(std::chrono::milliseconds delay, Action block);
		bool run(std::chrono::time_point<std::chrono::milliseconds> run_time);
		bool run();
		
		void print_summary() const;

	private:
		Cluster& cluster_;
		Scenario& scenario_;
		std::vector<Queue> all_queues_;
		
		// Stats
		int num_submitted_jobs_;
		int num_successful_jobs_;
		int num_failed_jobs_;
	public:
		
		static std::shared_ptr<spdlog::logger> file_logger;
	};

}


