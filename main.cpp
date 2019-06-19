#include <iostream>
#include "Parser.h"
#include <filesystem>
#include <direct.h>
#include "Scenario.h"
#include "Cluster_Simulation.h"

//const std::string SCENARIO_PATH = "D:\\OneDrive\\Documents\\C++\\simulator\\scenarios\\";
const std::string SCENARIO_PATH = "./scenarios/";
const std::string HOSTS_FILE = "hosts_final.json";
const std::string SCENARIO_FILE = "merged_Scenario_From_1000000_1000000.json";
//const std::string SCENARIO_PATH = "/scenarios/";

int main()
{	
	// Temporary CLI for input files
	std::string scenario_path;
	std::string host_path;
	int lines;
	std::cout << "##Cluster Simulator" << std::endl;
	std::cout << "Path to scenario file: ";
	std::cin >> scenario_path;
	std::cout << "Input the number of lines to read: ";
	std::cin >> lines;
	std::cout << "Path to host file: ";
	std::cin >> host_path;
	
	ClusterSimulator::Scenario scenario;
	ClusterSimulator::Cluster cluster;
		
	// Parse the given scenario and the cluster from json files.
	if (!host_path.empty() && !host_path.empty())
	{
		scenario = ClusterSimulator::Parser::parse_scenario(scenario_path, 50);
		cluster = ClusterSimulator::Parser::parse_cluster(host_path);
	}
	else
	{
		scenario = ClusterSimulator::Parser::parse_scenario(SCENARIO_PATH + SCENARIO_FILE, 50);
		cluster = ClusterSimulator::Parser::parse_cluster(SCENARIO_PATH + HOSTS_FILE);
	}

	// Start simulation
	ClusterSimulator::ClusterSimulation simulation{ scenario, cluster };
	simulation.run();

	// Print summary
	simulation.print_summary();



	//std::string current_working_dir(NPath);
	//std::cout << current_working_dir;	ClusterSimulator::ParseScenario(SCENARIO_PATH + "lines.json");

	//ClusterSimulator::ParseCluster(SCENARIO_PATH + HOSTS_FILE);
	//ClusterSimulator::parse_scenario(SCENARIO_PATH + SCENARIO_FILE);

	//test
	//double simulationTime = 7200;
	//int nodeNum = 8;
	//RandomAlgorithm algorithm{};

	//Simulation simulation{algorithm, simulationTime, nodeNum};

	//simulation.simulate();
}

