#include "controller.h"
#include "storage.h"

Controller::Controller(Storage *storage, bool is_pipelined)
{
	this->clock_cycle = 0;
	this->storage = storage;
	this->is_pipelined = is_pipelined;
	this->pc = 0x0;
	this->gprs = {0};

	// setup the other pipeline stages
	this->next = nullptr;
}

Controller::~Controller() { ; }

void Controller::run_for(int number)
{
	int i;
	for (i = 0; i < number; ++i) {
		this->advance();
	}
}

int Controller::get_clock_cycle() { return this->clock_cycle; }

std::array<int, GPR_NUM> Controller::get_gprs() {
	return this->gprs;
}

int Controller::get_pc() { return this->pc; }

void Controller::advance() {
	;
	// this->next->advance()
	++this->clock_cycle;
}
