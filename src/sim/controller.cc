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
	this->checked_out = {};
	this->gprs = {0};
	// grant side-door access
	this->id = SIDE;
}

void Controller::run_for(int number)
{
	int i;
	for (i = 0; i < number; ++i) {
		this->advance(WAIT);
	}
}

int Controller::get_clock_cycle() { return this->clock_cycle; }

std::array<int, GPR_NUM> Controller::get_gprs() { return this->gprs; }

void Controller::set_gprs(int index, int value) { this->gprs[index] = value; }

void Controller::set_pipelined(bool value) { this->is_pipelined = value; }

int Controller::get_pc() { return this->pc; }

InstrDTO *Controller::advance(Response p)
{
	InstrDTO *r;
	r = this->next->advance(p);
	++this->clock_cycle;

	return r;
}

void Controller::advance_helper()
{
	// TODO: check halt condition and call UI to refresh
}
