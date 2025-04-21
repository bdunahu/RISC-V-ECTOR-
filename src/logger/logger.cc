// Simulator for the RISC-V[ECTOR] mini-ISA
// Copyright (C) 2025 Siddarth Suresh
// Copyright (C) 2025 bdunahu

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "logger.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

LogLevel Logger::level = INFO;
Logger *Logger::logger_instance;

void Logger::setLevel(LogLevel level) { this->level = level; }

void Logger::log(LogLevel level, const string &message)
{
	if (level_to_int(level) > level_to_int(this->level)) {
		return;
	}

	time_t now = time(0);
	tm *timeinfo = localtime(&now);
	char timestamp[20];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

	ostringstream logEntry;
	logEntry << "[" << timestamp << "] " << level_to_string(level) << ": "
			 << message << endl;

	cout << logEntry.str();
}

Logger *Logger::getInstance()
{
	if (logger_instance == nullptr) {
		logger_instance = new Logger();
	}
	return logger_instance;
}

string Logger::level_to_string(LogLevel level)
{
	switch (level) {
	case DEBUG:
		return "DEBUG";
	case INFO:
		return "INFO";
	case WARNING:
		return "WARNING";
	case ERROR:
		return "ERROR";
	case CRITICAL:
		return "CRITICAL";
	default:
		return "UNKNOWN";
	}
}

int Logger::level_to_int(LogLevel level)
{
	switch (level) {
	case DEBUG:
		return 5;
	case INFO:
		return 4;
	case WARNING:
		return 3;
	case ERROR:
		return 2;
	default:
		return 1;
	}
}
