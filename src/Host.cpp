#include "../includes/Job.h"
#include "../includes/Host.h"
#include "../dependencies/spdlog/spdlog.h"
#include "../includes/Cluster_Simulation.h"

namespace ClusterSimulator
{
	int Host::id_gen_ = 0;
	std::random_device Host::rd_{};
	std::mt19937 Host::gen_(rd_());
	std::uniform_int_distribution<> Host::dist_(1, 6);

	void Host::execute_job(const Job& job)
	{
		if (slot_running_ + job.slot_required > max_slot)
			ClusterSimulation::log(LogLevel::err, 
				"Host {0}: Slot required for job {1} cannot be fulfilled with this host.", id, job.id);

		slot_running_ += job.slot_required;
		num_current_running_slots +=  job.slot_required;
		num_current_jobs ++;
	}

	void Host::exit_job(const Job& job)
	{
		slot_running_ -= job.slot_required;
		num_current_running_slots -=  job.slot_required;
		num_current_jobs --;
	}
}

template<> const std::vector<std::string> Utils::enum_strings<ClusterSimulator::HostStatus>::data = {
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