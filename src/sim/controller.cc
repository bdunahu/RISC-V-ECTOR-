#include "controller.h"
#include "response.h"
#include "storage.h"

Controller::Controller(Stage *stage, Storage *storage, bool is_pipelined)
	: Stage(stage)
{
	this->clock_cycle = 1;
	this->storage = storage;
	this->is_pipelined = is_pipelined;
	this->pc = 0x0;
	this->gprs = {0};
	// grant side-door access
	this->id = SIDE;
}

void Controller::run_for(int number)
{
	InstrDTO instr;
	int i;
	for (i = 0; i < number; ++i) {
		this->advance(instr);
	}
}

int Controller::get_clock_cycle() { return this->clock_cycle; }

std::array<int, GPR_NUM> Controller::get_gprs() { return this->gprs; }

int Controller::get_pc() { return this->pc; }

Response Controller::advance(InstrDTO &i)
{
	Response r;

	r = this->next->advance(i);
	++this->clock_cycle;
	return r;
}
