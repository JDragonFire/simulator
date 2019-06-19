#pragma once
#ifndef LOGGER_H
#define LOGGER_H
#include "Task.h"
#include "Utils.h"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>

class Logger
{
public:
	Logger();
	~Logger();
	void writeLog(Task& task);
	int createLogFile(const std::string& name);

	int clearLog();
private:
	std::vector<Task> logs_{};

};

#endif

