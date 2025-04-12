#include "definitions.h"
#include "gui.h"
#include "logger.h"
#include <QApplication>
#include <getopt.h>
#include <iostream>

static Logger *global_log = Logger::getInstance();

static std::string version_number = "v0.1";
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
	GUI w;
	w.show();
	return a.exec();
}
