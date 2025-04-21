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

#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <sstream>
using namespace std;

enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

class Logger
{
  public:
	static Logger* getInstance();

	~Logger() = default;

	/**
	 * Do not allow copies.
	 */
	Logger(const Logger& obj) = delete;

	/**
	 * Set the log level.
	 * @param the log level to set to.
	 */
	void setLevel(LogLevel);
	/**
	 * Log a message at a certain log level.
	 * @param The level to log this message. If the level is lower than the
	 * level set by `setLevel`, then the message is not logged.
	 * @param The message to log.
	 */
	void log(LogLevel, const string &);

  private:
	Logger() = default;
	static Logger* logger_instance;
	static LogLevel level;
	static string level_to_string(LogLevel);
	static int level_to_int(LogLevel);
};

#endif /* LOGGER_H_INCLUDED */
