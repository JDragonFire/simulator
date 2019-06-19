#pragma once
#include "Logger.h"
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime>


Logger::Logger()
{
}


Logger::~Logger()
{
}

void Logger:: writeLog(Task& task) 
{
	//TODO:
	//setEnergy(task);
	logs_.push_back(task);
}

int Logger:: createLogFile(const std::string& name) {
	//TODO:
	std::ofstream out_log("logs.txt");
	

	//sort(logs_.begin(), logs_.end(), compareStart);
	//stable_sort(logs_.begin(), logs_.end(), compareNum);

	//double numAllTask = ((double)logs_.back().id) + 1;
	double numAllTask = 0;
	double numComplete = 0;
	double numDrop = 0;
	double numFail = 0;

	//for (unsigned int i = 0; i < logs_.size(); i++) 
	//{
	//	numComplete++;
	//}


	//out_log << "task#|task ID|arrival time|deadline|core|start time|finish time|exe time|status|energy" << endl;
	//out_log << "Task ID    Node ID    Start Time    Excution   Time   Finished Time" << endl;
	for (unsigned int i = 0; i < logs_.size(); i++) {
		out_log << logs_.at(i).print();
		numComplete++;
	}

	auto currentTime = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(currentTime);
	char time_string[25];
	ctime_s(time_string, 25, &time);

	out_log << "Simulation Start Time : ";
	out_log<< time_string << std::endl;
	out_log<<"Total number of logs : "<< numComplete<< std::endl;
	out_log.close();

	return 0;
}
int Logger::clearLog()
{
	logs_.clear();
	return 0;
}
