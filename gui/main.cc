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

#include "gui.h"
#include "logger.h"
#include "pipe_spec.h"
#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <getopt.h>
#include <iostream>

static Logger *global_log = Logger::getInstance();

static std::string version_number = "v0.8";
// clang-format off
static std::string banner =
  "    _/_/_/    _/_/_/    _/_/_/    _/_/_/   \n"
  "   _/    _/    _/    _/        _/          \n"
  "  _/_/_/      _/      _/_/    _/           \n"
  " _/    _/    _/          _/  _/            \n"
  "_/    _/  _/_/_/  _/_/_/      _/_/_/       \n"
  "                                           \n"
  "                                           \n"
  "               _/_/                                                      _/_/  \n"
  "  _/      _/  _/    _/_/_/_/    _/_/_/  _/_/_/_/_/    _/_/    _/_/_/      _/   \n"
  " _/      _/  _/    _/        _/            _/      _/    _/  _/    _/    _/    \n"
  "_/      _/  _/    _/_/_/    _/            _/      _/    _/  _/_/_/      _/     \n"
  " _/  _/    _/    _/        _/            _/      _/    _/  _/    _/    _/      \n"
  "  _/      _/    _/_/_/_/    _/_/_/      _/        _/_/    _/    _/    _/       \n"
  "         _/_/                                                      _/_/    ";
// clang-format on
static void print_version_number()
{
	std::cout << banner << version_number << '\n';
}

static void err()
{
	std::cerr << "Usage:\n\trisc_vector [OPTIONS]\nOptions:\n\t--debug,\t-d: "
				 "turn on verbose output\n\t--version,\t-v: print the version "
				 "information and exit\n"
			  << std::endl;
}

static void parseArguments(int argc, char **argv)
{
	struct option long_options[] = {
		{"debug", no_argument, 0, 'd'}, {0, 0, 0, 0}};

	int opt;

	while ((opt = getopt_long(argc, argv, "d", long_options, NULL)) != -1) {
		switch (opt) {
		case 'd':
			global_log->setLevel(DEBUG);
			global_log->log(DEBUG, "DEBUG output enabled.");
			break;
		default:
			err();
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char **argv)
{
	print_version_number();
	global_log->log(INFO, "Initializing...");

	parseArguments(argc, argv);

	global_log->log(INFO, "Starting QT...");
	QApplication a(argc, argv);

	int fId = QFontDatabase::addApplicationFont(
		":/resources/BigBlueTermPlusNerdFontMono-Regular.ttf");
	QFile ssf(":/resources/styles.qss");
	QString f = QFontDatabase::applicationFontFamilies(fId).at(0);

	ssf.open(QFile::ReadOnly);
	QString ss = QLatin1String(ssf.readAll());
	a.setStyleSheet(ss);

	GUI w;
	w.show();
	return a.exec();
}
